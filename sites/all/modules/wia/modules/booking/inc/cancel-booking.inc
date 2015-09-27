<?php
/*****
#action: cancel_booking_form
#description: cancel booking form
#author: PKS
#created: 05 Aug 2015
*****/

function cancel_booking_form($form, &$form_state) {	
	
	global $user;
	
	$orderId=arg(1);
	
	$order=commerce_order_load($orderId);	
	
	$form['#tree']=TRUE;
	
	$form['order_id'] = array(
		'#type' => 'hidden',
		'#default_value' => $orderId,		
	);	
	
	if($order && $order->uid==$user->uid && strtolower($order->status)=='completed'){		
		
		$lineItems=$order->commerce_line_items;	
			
		if($lineItems){
			
			foreach($lineItems['und'] as $_lineItem){				
				
				$lineItemId=$_lineItem['line_item_id'];		
								
				$rs=get_slots_by_order($orderId, $lineItemId,$user->uid);	
			
				$curTS=time();
								
				if($rs){
					foreach($rs as $_slot){
					
						$form['time_slot'][$orderId][$lineItemId][$_slot->id] = array(
							'#type' => 'checkbox',
							'#title' => $_slot->name,
							'#prefix' => '<div class="checkbox checkbox-info">',
							'#suffix' => '</div>'							
						);		
						
						
						//booking start time validation
						$slotDetail=get_slot_details_by_id($_slot->tid);
						$slotStTime=$slotDetail->field_start_time['und'][0]['value'];
						$bookingStDateTime=$_slot->date.''.$slotStTime;
						$bookingStDateTimeTS=strtotime($bookingStDateTime);
												
						if($_slot->status!='Booked' || $curTS>$bookingStDateTimeTS){
								
							$form['time_slot'][$orderId][$lineItemId][$_slot->id]['#attributes']['disabled']='disabled';
						}
						
						$form['time_slot'][$orderId][$lineItemId][$_slot->id]['#attributes']['class'][]='styled';	
					}
				}
				
			}
		}
		
	}	
	
	$form['submit'] = array(
		'#value' => 'Cancel Booking',
		'#type' => 'submit',
		'#attributes' => array('onclick' => 'if(!confirm("Are you sure to cancel these booking(s)?")){return false;}')	
	);		
	
	$form['submit']['#attributes']['class'][]='btn cancel-booking';		
	
	return $form;	
} 

/*****
#action: cancel_booking_form_validate
#description: validate cancel booking.
#author: PKS
#created: 05 aug 2015
*****/
function cancel_booking_form_validate($form, &$form_state) {
	
	global $user;
	
	$orderId=$form_state['values']['order_id'];	
	$order=commerce_order_load($orderId);	
	
	if(empty($order))
	form_set_error('order_id', 'Invalid order id.');
	
	
	if($order->uid!=$user->uid)
	form_set_error('order_id', 'Access denied.');
	
	if(strtolower($order->status)!='completed')
	form_set_error('order_id', 'Invalid order status.');		
	
}


/*****
#action: cancel_booking_form_submit
#description: cancel the booking.
#author: PKS
#created: 5 Aug 2015
*****/
function cancel_booking_form_submit($form, &$form_state) {
    	
	global $user;
	
	$orderId=$form_state['values']['order_id'];	
	$order=commerce_order_load($orderId);
		
	if($order && $order->uid==$user->uid){
			
		$bookings=$form_state['values']['time_slot'][$orderId];
		
		$totalAmountToRefund=0;
		$isCanceled=false;
		$arrCanceledSlots=array();
			
		if($bookings){			
			
			foreach($bookings as $lineItemId => $_bookings){
				
				if($_bookings){				
					
					foreach($_bookings as $_bookingId => $_cancelFlag){					
					
						if($_cancelFlag && $_cancelFlag==1){	
						
							$slotB=get_slot_detail_by_booking_id($_bookingId,$orderId,$lineItemId,$user->uid);		
													
							if($slotB && $slotB['slot_booking']->status=='Booked'){
								
								$slotBookingBasePrice=$slotB['slot_booking']->slot_price;
								$vat=$slotB['slot_booking']->vat;
																
								$priceToRefund=$slotBookingBasePrice+($vat*$slotBookingBasePrice);								
								
								$bookingDate=$slotB['slot_booking']->date;
								
								$bookingFromTime=$slotB['slot_meta']->field_start_time['und'][0]['value'];
								$bookingToTime=$slotB['slot_meta']->field_end_time['und'][0]['value'];							
								$bookingFromTS=strtotime($bookingDate." ".$bookingFromTime);							
								
								$curTS=time();
								
								if($bookingFromTS>=$curTS){
									
									$remainTS=$bookingFromTS-$curTS;
									$remainH=floor($remainTS/3600);									
									$refundRule=get_refund_rule_by_hours($remainH);	
								
									if($refundRule && !empty($refundRule->amount)){
										
										 $priceToRefund=$priceToRefund-($priceToRefund*($refundRule->amount/100));
									}		
																
									//update booking status
									$urs=db_update('slot_bookings')->fields(array(
										'notes' => 'Booking is canceled',	
										'cancellation_date' => date("Y-m-d H:i:s"),
										'refund_amount' => $priceToRefund,
										'status' => 'Canceled'	
										))		
										->condition('id',$_bookingId, '=')
										->execute();
										
									if($urs){								
										$totalAmountToRefund+=$priceToRefund;	
										$isCanceled=true;
										$arrCanceledSlots[]=$slotB;
										
									}
									
								}								
							
							}					
							
						}
					}
				}				
							
			}
		}
		
		if($isCanceled){		
		
			$toRefund=$order->field_refund_amount['und'][0]['value'];
			$rstatus=$order->field_refund_status['und'][0]['value'];
			if($rstatus=='Refund'){
				$toRefund=$toRefund+$totalAmountToRefund;
				
			}else{
				$toRefund=$totalAmountToRefund;
			}
			
			
			$order->field_refund_amount['und'][0]['value']=$toRefund;
			$order->field_refund_status['und'][0]['value']='Refund';		
			commerce_order_save($order);
			
			$currencySymbol=CURRENCY_SYMBOL;
			
			drupal_set_message(t("Your cancellation request has been proccessed successfully. The amount {$currencySymbol}{$totalAmountToRefund} will be refunded to your account."), 'success');
			
			//send cancellation email
			send_booking_cancellation_email($order,array('amount'=>$totalAmountToRefund,'slots'=>$arrCanceledSlots));
			
			
		}else{
			drupal_set_message(t("Error occurred during processing of your request. Please select valid booked slot(s) to cancel bookings."), 'error');
		
		}
		
	}else{		
		drupal_set_message(t('An error occurred and processing did not complete.'), 'error');
	}	

} 


/*--admin actions to cancel bookings--*/

/*****
#action: cancel_booking_form
#description: cancel booking form
#author: PKS
#created: 05 Aug 2015
*****/

function admin_cancel_booking_form($form, &$form_state) {	
	
	global $user;
	
	$orderId=arg(2);
	
	$order=commerce_order_load($orderId);	
	
	$form['#tree']=TRUE;
	
	$form['order_id'] = array(
		'#type' => 'hidden',
		'#default_value' => $orderId,		
	);
	
	
	$form['refund_type'] = array(
		'#title' => 'Refund Type',
		'#type' => 'radios',
		'#default_value' => 'fr',
		'#options' => array('fr'=>'Full Refund','arp'=>'Apply Refund Policy'),
		'#required' => 'true',
	);	
	
	if($order && strtolower($order->status)=='completed'){		
		
		$lineItems=$order->commerce_line_items;	
			
		if($lineItems){
			
			foreach($lineItems['und'] as $_lineItem){				
				
				$lineItemId=$_lineItem['line_item_id'];		
								
				$rs=get_slots_by_order($orderId, $lineItemId,$order->uid);	
				
				$curTS=time();
								
				if($rs){
					foreach($rs as $_slot){
					
						$form['time_slot'][$orderId][$lineItemId][$_slot->id] = array(
							'#type' => 'checkbox',
							'#title' => $_slot->name,
							'#prefix' => '<div class="checkbox checkbox-info">',
							'#suffix' => '</div>'							
						);		
						
						
						//booking start time validation
						$slotDetail=get_slot_details_by_id($_slot->tid);
						$slotStTime=$slotDetail->field_start_time['und'][0]['value'];
						$bookingStDateTime=$_slot->date.''.$slotStTime;
						$bookingStDateTimeTS=strtotime($bookingStDateTime);
												
						if($_slot->status!='Booked' || $curTS>$bookingStDateTimeTS){
								
							$form['time_slot'][$orderId][$lineItemId][$_slot->id]['#attributes']['disabled']='disabled';
						}
						
						$form['time_slot'][$orderId][$lineItemId][$_slot->id]['#attributes']['class'][]='styled';	
					}
				}
				
			}
		}
		
	}	
	
	$form['submit'] = array(
		'#value' => 'Cancel Booking',
		'#type' => 'submit',
		'#attributes' => array('onclick' => 'if(!confirm("Are you sure to cancel these booking(s)?")){return false;}')	
	);		
	
	$form['submit']['#attributes']['class'][]='btn cancel-booking';		
	
	return $form;	
} 

/*****
#action: admin_cancel_booking_form_validate
#description: validate cancel booking.
#author: PKS
#created: 05 aug 2015
*****/
function admin_cancel_booking_form_validate($form, &$form_state) {
	
	global $user;
	
	$orderId=$form_state['values']['order_id'];	
	$order=commerce_order_load($orderId);	
	
	if(empty($order))
	form_set_error('order_id', 'Invalid order id.');	
	
	if(strtolower($order->status)!='completed')
	form_set_error('order_id', 'Invalid order status.');		
	
}


/*****
#action: admin_cancel_booking_form_submit
#description: cancel the booking by admin.
#author: PKS
#created: 5 Aug 2015
*****/
function admin_cancel_booking_form_submit($form, &$form_state) {
    	
	global $user;
	
	$orderId=$form_state['values']['order_id'];	
	$order=commerce_order_load($orderId);
		
	if($order){
			
		$bookings=$form_state['values']['time_slot'][$orderId];
		$refundType=$form_state['values']['refund_type'];	
		
		$totalAmountToRefund=0;
		$isCanceled=false;
		$arrCanceledSlots=array();
			
		if($bookings){			
			
			foreach($bookings as $lineItemId => $_bookings){
				
				if($_bookings){				
					
					foreach($_bookings as $_bookingId => $_cancelFlag){					
					
						if($_cancelFlag && $_cancelFlag==1){	
						
							$slotB=get_slot_detail_by_booking_id($_bookingId,$orderId,$lineItemId,$order->uid);		
													
							if($slotB && $slotB['slot_booking']->status=='Booked'){
								
								$slotBookingBasePrice=$slotB['slot_booking']->slot_price;
								$vat=$slotB['slot_booking']->vat;
																
								$priceToRefund=$slotBookingBasePrice+($vat*$slotBookingBasePrice);								
								
								$bookingDate=$slotB['slot_booking']->date;
								
								$bookingFromTime=$slotB['slot_meta']->field_start_time['und'][0]['value'];
								$bookingToTime=$slotB['slot_meta']->field_end_time['und'][0]['value'];							
								$bookingFromTS=strtotime($bookingDate." ".$bookingFromTime);							
								
								$curTS=time();
								
								if($bookingFromTS>=$curTS){									
									
									if($refundType!='fr'){									
										
										$remainTS=$bookingFromTS-$curTS;
										$remainH=floor($remainTS/3600);									
										$refundRule=get_refund_rule_by_hours($remainH);	
									
										if($refundRule && !empty($refundRule->amount)){
											
											 $priceToRefund=$priceToRefund-($priceToRefund*($refundRule->amount/100));
										}											
									}
									
									//update booking status
									$urs=db_update('slot_bookings')->fields(array(
										'notes' => 'Booking is canceled',	
										'cancellation_date' => date("Y-m-d H:i:s"),
										'refund_amount' => $priceToRefund,
										'status' => 'Canceled'	
										))		
										->condition('id',$_bookingId, '=')
										->execute();										
																	
									$totalAmountToRefund+=$priceToRefund;	
									$isCanceled=true;
									$arrCanceledSlots[]=$slotB;
										
									
									
								}								
							
							}					
							
						}
					}
				}				
							
			}
		}
		
		if($isCanceled){		
		
			$toRefund=$order->field_refund_amount['und'][0]['value'];
			$rstatus=$order->field_refund_status['und'][0]['value'];
			if($rstatus=='Refund'){
				$toRefund=$toRefund+$totalAmountToRefund;
				
			}else{
				$toRefund=$totalAmountToRefund;
			}
			
			$order->field_refund_amount['und'][0]['value']=$toRefund;
			$order->field_refund_status['und'][0]['value']='Refund';		
			commerce_order_save($order);
			
			$currencySymbol=CURRENCY_SYMBOL;
			
			drupal_set_message(t("The cancellation request has been proccessed successfully. Please make a refund of amount {$currencySymbol}{$totalAmountToRefund} to the customer account."), 'success');
			
			//send cancellation email
			send_admin_booking_cancellation_email($order,array('amount'=>$totalAmountToRefund,'slots'=>$arrCanceledSlots,'refund_type'=>$refundType));
			
			
		}else{
			drupal_set_message(t("Error occurred during processing of your request. Please select valid booked slot(s) to cancel bookings."), 'error');
		
		}
		
	}else{		
		drupal_set_message(t('An error occurred and processing did not complete.'), 'error');
	}	

} 
/*--admin actions to cancel bookings--*/