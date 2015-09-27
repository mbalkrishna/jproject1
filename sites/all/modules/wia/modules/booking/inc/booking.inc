<?php
if(!isset($_SESSION))
session_start();
/*****
#action: booking_lock_my_slots
#description: lock current selected user slots
#author: PKS
#created: 17 July 2015
*****/
function booking_lock_my_slots() {
	
	global $user;	
	if($_POST && !empty($_POST['slots']) && !empty($_POST['product_id'])){
			
		$arrReqSlots=json_decode($_POST['slots']);
		$productId=$_POST['product_id'];
		
		$arrSlots=null;
		foreach($arrReqSlots as $_slots){
			
			$tmpArrSlots=explode("-",$_slots);
			$tmpProductId=$tmpArrSlots[0];
			$slotId=$tmpArrSlots[1];
			$dateTS=$tmpArrSlots[2];
			
			if($productId==$tmpProductId)//check the correct slot			
			$arrSlots[$dateTS][]=$slotId;
		}
		
		ksort($arrSlots);
				
		$verifError=true;
		$verifMsg="<p class='msg_head'>following error(s) occured during verifying your selection. please correct the below error(s) and apply again:</p>";
		$dbSlots=null;
		$dbSlotPrices=null;		
				
		if($arrSlots){			
			
			//delete prev locked slots if any
			unlock_curret_slots();		
			
			$dbSlots=get_time_slots('list');
			$dbSlotPrices=get_slot_prices($productId);
			
			foreach($arrSlots as $dateTS => $_slots){				
				foreach($_slots as $slotId){
					
					$date=date('Y-m-d',$dateTS);
					
					$msgDate=date("l jS M",$dateTS);
					$slotName=$dbSlots[$slotId];
					
					//get slot master details
					$slotDB=get_slot_details_by_id($slotId);					
					$slotStTime=$slotDB->field_start_time['und'][0]['value'];
					$dateTS2=strtotime($date.' '.$slotStTime);
					//validate past date bookings
					
					if($dateTS2<strtotime(date("Y-m-d H:i"))){
						$validationError=true;				
						$verifMsg.="<p>you can not book the slot <strong>{$msgDate} - {$slotName}</strong>. Because the slot is either unavailable or date is passed.</p>";
					}				
									
					//validate availability
					$srs = db_select('slot_bookings', 'sb')
					->fields('sb')
					->condition('product_id', $productId, '=')
					->condition('slot_id', $slotId, '=')
					->condition('date',$date, '=')
					->condition('status',array('Booked','Unavailable','Processing'), 'IN')
					->execute()
					->fetch();
					
					if($srs){
						$validationError=true;
						$verifMsg.="<p>the slot <strong>$msgDate - {$slotName}</strong> is booked/unavailable.</p>";
					}				
				}
			}
		}else{
			$validationError=true;
			$verifMsg.="<p>please select at least one slot for further processing.</p>";
		}
		
		if($validationError){			
			return array('message'=>$verifMsg,'type'=>'error','data'=>'','scope'=>'popup');		
		}		
		
		
		//lock the slots and prepare front message
		$lockedSlots=NULL;
		$slotTotalPrice=0;
		
		$formatedSlotsData="<ul class='lowercase' style='list-style:none;margin:0;padding:0;'>";
		$formatedSlotsData2="<ul class='lowercase' style='list-style:none;margin:0;padding:0;'>";
		
		$tmpDateTS=NULL;
		foreach($arrSlots as $dateTS => $_slots){				
			foreach($_slots as $slotId){		
				
				$date=date('Y-m-d',$dateTS);
				
				$msgDate=date("l jS M",$dateTS);
				$slotName=$dbSlots[$slotId];	
				
				$status='Processing';			
			
				
				$irs=db_insert('slot_bookings')->fields(array(
				'product_id' =>$productId,
				'slot_id' =>$slotId,
				'date' => $date,
				'slot_price' =>  $dbSlotPrices[$slotId]['price'],
				'status' => $status,
				'notes' => 'Booking is initiated',
				'uid' => $user->uid,
				'added_date'=>time()))
				->execute();
			
				if($irs){
					$lockedSlots[]=$irs;	
					
					$slotTotalPrice=$slotTotalPrice + $dbSlotPrices[$slotId]['price'];
						
					if($tmpDateTS!=$dateTS){
						$formatedSlotsData.="<li class='head'><strong>{$msgDate}</strong></li>";	
						$formatedSlotsData2.="<li class='head'><strong>{$msgDate}</strong></li>";						
						$tmpDateTS=$dateTS;
					}
					
					$formatedSlotsData.="<li>{$slotName}</li>";
					$slotNameWithPrice=CURRENCY_SYMBOL.$dbSlotPrices[$slotId]['price'];
					$formatedSlotsData2.="<li><span class='title'>{$slotName}</span><span class='price'>{$slotNameWithPrice}</span></li>";
				}
			
			}
		}
		$formatedSlotsData.="</ul>";
		
		$arrSlotDetails=array('slots'=>$lockedSlots,
			'slot_total_price'=>$slotTotalPrice,
			'slot_descriptions'=>$formatedSlotsData
		);
		
		$_SESSION['cart_extra_info']=$arrSlotDetails;
		
		//$lockedSlots=json_encode($lockedSlots);			
		return array('message'=>$formatedSlotsData2,'type'=>'success','data'=>$arrSlotDetails,'scope'=>'page');		
		
	}
	
	return array('message'=>"Invalid status passed.",'type'=>'error','data'=>'','scope'=>'popup');
	
}

/*****
#action: booking_lock_my_slots_response_handler
#description: response for slot locking
#author: PKS
#created: 17 July 2015
*****/
function booking_lock_my_slots_response_handler($res){
	echo json_encode($res);	

}

/*****
#action: booking_booking_details
#description: show booking details
#author: PKS
#created: 30 July 2015
*****/
function booking_booking_details($orderId=null){
	
	global $base_url;
	
	if(empty($orderId))
	$orderId=arg(1);
	
	if(empty($orderId)){return "<div class='invalid-order error messages'>invalid order.</div>";}
	
	$order=commerce_order_load($orderId);
	$out='';	
	if($order){
		
		global $user;
		if($user->uid!=$order->uid){
			return "<div class='invalid-order error messages'>invalid order.</div>";
		}		
		
		$orderStatus=$order->status;
		$orderDate=date("d/m/y",$order->created);
		$orderTotal=commerce_currency_format($order->commerce_order_total['und'][0]['amount']);
		
		$vatTotal=commerce_currency_format($order->commerce_order_total['und'][0]['data']['components'][1]['price']['amount']);
		
		$currecnySyb=CURRENCY_SYMBOL;
		
		$lineItems=$order->commerce_line_items;
		
		
		$bilingInfo=commerce_customer_profile_load($order->commerce_customer_billing['und'][0]['profile_id']);
		$address='';
		if($bilingInfo){
			
			if($bilingInfo->commerce_customer_address['und'][0]['name_line'])
			$address.=$bilingInfo->commerce_customer_address['und'][0]['name_line'].", ";
			
			if($bilingInfo->commerce_customer_address['und'][0]['locality'])
			$address.=$bilingInfo->commerce_customer_address['und'][0]['locality'].", ";
			
			if($bilingInfo->commerce_customer_address['und'][0]['postal_code'])
			$address.="postcode - ".$bilingInfo->commerce_customer_address['und'][0]['postal_code'].", ";
			
			$address=trim($address,", ");
		}
	
		
		$out.="<div id='order_detail_block'>";
		$out.="<div class='cart-table1'>";
		$out.="<h2>order detail: #{$orderId}</h2>";	
		$out.="<table width='100%' border='0' cellspacing='0' cellpadding='0' class='cart-table'>";	  
		
		$out.="<tr>";
		$out.="<td>order no.</td>";	
		$out.="<td>date of booking</td>";
		$out.="<td>description</td>";		
		$out.="<td>status</td>";
		$out.="<td align='right'>total amount</td>";
		$out.="<td>&nbsp;</td>";
		$out.="</tr>";
		
		$out.="<tr>";
		$out.="<td><span class='label-mob'>order no.</span>#{$orderId}</td>";
		$out.="<td><span class='label-mob'>date of booking</span>{$orderDate}</td>";
		$out.="<td><span class='label-mob'>description</span>{$address}</td>";
		
		$out.="<td><span class='label-mob'>status</span>{$orderStatus}</td>";
		$out.="<td align='right'> <span class='label-mob'>total amount</span>{$currecnySyb}{$orderTotal}</td>";
		$out.="<td align='right'></td>";
		$out.="</tr>";
		
		$out.="<tr>";
		$out.="<td style='border:none' colspan='6'>&nbsp;</td>";	
		$out.="</tr>";
	
		$out.="<tr>";
		$out.="<td style='border:none' colspan='6'>&nbsp;</td>";	
		$out.="</tr>";
	
		$out.="<tr>";
		$out.="<td width='10%'>image</td>";
		$out.="<td width='30%'>venue</td>";
		$out.="<td width='20%'>time slots</td>";
		$out.="<td width='20%'>amenities</td>";
		$out.="<td align='right'>amount</td>";
		$out.="<td>&nbsp;</td>";
		$out.="</tr>";
		
		if($lineItems){
			foreach($lineItems['und'] as $_lineItem){
				
				$line_item_id=$_lineItem['line_item_id'];		
				
				$rowItem=commerce_line_item_load($line_item_id);			
				$pwrapper = entity_metadata_wrapper('commerce_product', $rowItem->commerce_product['und'][0]['product_id']);		
				
				$itemPrice=commerce_currency_format($rowItem->commerce_unit_price['und'][0]['amount']);
				$itemPriceDisplay=CURRENCY_SYMBOL." ".$itemPrice;		
				$slotDec=$rowItem->data['slot_details']['slot_descriptions'];
				
				$itemTitle=$pwrapper->title->value();
				$roomName=$pwrapper->field_room_name->value();
				
				$out.="<tr>";        
				$out.="<td>";
				
				$fImg = $pwrapper->field_featured_image->value();
				 
				if($fImg && $fImg['uri']){
					
					$img = array(
					'style_name' => 'cart_thumbnail',
					'path' => $fImg['uri'],
					'alt' => $fImg['alt'],
					'title' => $fImg['title']			
					);
				
					$out.=theme('image_style',$img);
				}else{
					$out.='<img src="'.$base_url.'/'.$theme_path.'/images/no-image.jpg" width="50"/>';
				}                
				
			
				$out.="</td>";
				$out.="<td><span class='label-mob'>venue</span><strong>{$itemTitle}";
				$out.="&nbsp;({$roomName})</strong>";
				
				
				if($pwrapper->field_full_address->value()){
					$out.="<br/>".$pwrapper->field_full_address->value();
				}              
				
				$out.="</td>";
				$out.="<td><span class='label-mob'>time slots</span>{$slotDec}</td>";

				$amenities = $rowItem->data['req_data']['amenities'];
				$ame=get_amenites_by_aids(explode(",",$amenities),'list');
						
				$out.="<td><span class='label-mob'>amenities</span>{$ame}</td>";
			
				$out.="<td align='right'><span class='label-mob'>amount</span>{$currecnySyb}{$itemPrice}</td>";		
				$out.="<td align='right'></td>";			
				$out.="</tr>";
				
			}
		}	
	
		$out.="<tr class='grand-total'>";
		$out.="<td class='remove'></td>";
		$out.="<td class='remove'></td>";
		$out.="<td class='remove'></td>";
		$out.="<td>vat</td>";
		$out.="<td align='right'>{$currecnySyb}{$vatTotal}</td>";	
		$out.="<td align='right'></td>";
		$out.="</tr>";
		
		$out.="<tr class='grand-total'>";
		$out.="<td class='remove'></td>";
		$out.="<td class='remove'></td>";
		$out.="<td class='remove'></td>";
		$out.="<td>total</td>";
		$out.="<td align='right'>{$currecnySyb}{$orderTotal}</td>";	
		$out.="<td align='right'></td>";
		$out.="</tr>";
		
		$out.="</table>";	
		
		$out.="</div>";		
		$out.="</div>";	
		
		$out.="<div class='cart-btn'>";
		$out.="<a href='{$base_url}/download-booking-details/{$orderId}/download_pdf' class='btn1 left' target='_blank'>download pdf</a>";
		$out.="<a href='#' class='btn1 left' onclick=\"print_block('order_detail_block')\" style='margin-right:10px'>print order</a>";
		$out.="<a href='javascript:window.history.back();' class='btn left'>back</a>";
		$out.="</div>";
		
	}else{
		$out.="<div class='invalid-order error messages'>invalid order.</div>";
	}
	
		
	return $out;

}




/*****
#action: booking_booking_details
#description: show booking details
#author: PKS
#created: 30 July 2015
*****/
function booking_download_booking_details($orderId=null){
	
	global $base_url,$theme_path;
	
	if(empty($orderId))
	$orderId=arg(1);
	
	if(empty($orderId)){return "<div class='invalid-order error messages'>invalid order.</div>";}
	
	$order=commerce_order_load($orderId);
	$out='';	
	if($order){
		
		global $user;
		if($user->uid!=$order->uid){
			return "<div class='invalid-order error messages'>invalid order.</div>";
		}		
		
		$orderStatus=$order->status;
		$orderDate=date("d/m/Y",$order->created);
		$orderTotal=commerce_currency_format($order->commerce_order_total['und'][0]['amount']);
		
		$vatTotal=commerce_currency_format($order->commerce_order_total['und'][0]['data']['components'][1]['price']['amount']);
		
		$currecnySyb=CURRENCY_SYMBOL;
		
		$lineItems=$order->commerce_line_items;
		
		
		$bilingInfo=commerce_customer_profile_load($order->commerce_customer_billing['und'][0]['profile_id']);
		$address='';
		if($bilingInfo){
			
			if($bilingInfo->commerce_customer_address['und'][0]['name_line'])
			$address.=$bilingInfo->commerce_customer_address['und'][0]['name_line'].", ";
			
			if($bilingInfo->commerce_customer_address['und'][0]['locality'])
			$address.=$bilingInfo->commerce_customer_address['und'][0]['locality'].", ";
			
			if($bilingInfo->commerce_customer_address['und'][0]['postal_code'])
			$address.="Pin - ".$bilingInfo->commerce_customer_address['und'][0]['postal_code'].", ";
			
			$address=trim($address,", ");
		}
	
		$out.='<page backtop="10mm" backbottom="10mm" backleft="10mm" backright="10mm">';
  		
		$out.='<page_header>';
		$out.='<table style="width:100%; background: #f3f3f3;" border="0" cellspacing="0" cellpadding="0">';
		$out.='<tr>';
      	$out.='<td><img src="'.$base_url.'/'.$theme_path.'/images/pdf-header.jpg" alt="" /></td>';
    	$out.='</tr>';
    	$out.='</table>';
      	$out.='</page_header>';
		
		$out.="<br><br><br><h2>Order Detail: #{$orderId}</h2><br>";	
	
		$out.="<table border='0' cellspacing='0' cellpadding='0' style='width:100%; border-left:solid 1px #aaaaaa; border-top:solid 1px #aaaaaa;'>";	  
		
		$out.='<col style="width: 10%">';
		$out.='<col style="width: 30%">';
		$out.='<col style="width: 30%">';
		$out.='<col style="width: 20%">';
		$out.='<col style="width: 10%">';
		
		$out.="<tr>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;border-top:solid 1px #aaaaaa;border-left:solid 1px #aaaaaa;padding:5px;'>Order#</td>";	
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;border-top:solid 1px #aaaaaa;padding:5px;'>Description</td>";	
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;border-top:solid 1px #aaaaaa;padding:5px;'>Date of Booking</td>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;border-top:solid 1px #aaaaaa;padding:5px;'>Status</td>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;border-top:solid 1px #aaaaaa;padding:5px;' align='right'>Total Amount</td>";
		$out.="</tr>";
		
		$out.="<tr>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;border-left:solid 1px #aaaaaa;padding:5px;'>#{$orderId}</td>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'>{$address}</td>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'>{$orderDate}</td>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'>{$orderStatus}</td>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;' align='right'>{$currecnySyb}{$orderTotal}</td>";
		$out.="</tr>";
		
		$out.="<tr>";
		$out.="<td style='border:none' colspan='5'>&nbsp;</td>";	
		$out.="</tr>";
	
		$out.="<tr>";
		$out.="<td style='border:none' colspan='5'>&nbsp;</td>";	
		$out.="</tr>";
	
		$out.="<tr>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa; border-top:solid 1px #aaaaaa;border-left:solid 1px #aaaaaa;padding:5px;'>Image</td>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;border-top:solid 1px #aaaaaa;padding:5px;'>Venue</td>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;border-top:solid 1px #aaaaaa;padding:5px;'>Time slots</td>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;border-top:solid 1px #aaaaaa;padding:5px;'>Amenities</td>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;border-top:solid 1px #aaaaaa;padding:5px;' align='right'>Amount</td>";
		$out.="</tr>";
		
		if($lineItems){
			foreach($lineItems['und'] as $_lineItem){
				
				$line_item_id=$_lineItem['line_item_id'];		
				
				$rowItem=commerce_line_item_load($line_item_id);			
				$pwrapper = entity_metadata_wrapper('commerce_product', $rowItem->commerce_product['und'][0]['product_id']);		
				
				$itemPrice=commerce_currency_format($rowItem->commerce_unit_price['und'][0]['amount']);
				$itemPriceDisplay=CURRENCY_SYMBOL." ".$itemPrice;		
				$slotDec=$rowItem->data['slot_details']['slot_descriptions'];
				
				$itemTitle=$pwrapper->title->value();
				$roomName=$pwrapper->field_room_name->value();
				
				$out.="<tr>";        
				$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;border-left:solid 1px #aaaaaa;padding:5px;'>";
				
				$fImg = $pwrapper->field_featured_image->value();
				 
				if($fImg && $fImg['uri']){
					
					$img = array(
					'style_name' => 'cart_thumbnail',
					'path' => $fImg['uri'],
					'alt' => $fImg['alt'],
					'title' => $fImg['title']			
					);
				
					$out.=theme('image_style',$img);
				}else{
					$out.='<img src="'.$base_url.'/'.$theme_path.'/images/no-image.jpg" width="50"/>';
				}                
				
			
				$out.="</td>";
				$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'>{$itemTitle}";
				$out.="&nbsp;({$roomName})";
				
				
				if($pwrapper->field_full_address->value()){
					$out.="<br/>".$pwrapper->field_full_address->value();
				}              
				
				$out.="</td>";
				$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'>{$slotDec}</td>";

				$amenities = $rowItem->data['req_data']['amenities'];
				$ame=get_amenites_by_aids(explode(",",$amenities),'list');
						
				$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'>{$ame}</td>";
			
				$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;' align='right'>{$currecnySyb}{$itemPrice}</td>";		
				$out.="</tr>";
				
			}
		}	
	
		$out.="<tr>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa;border-left:solid 1px #aaaaaa;padding:5px;'></td>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa;padding:5px;'></td>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa;padding:5px;'></td>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa;padding:5px;'>Vat</td>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;' align='right'>{$currecnySyb}{$vatTotal}</td>";	
		$out.="</tr>";
		
		$out.="<tr>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa;border-left:solid 1px #aaaaaa;padding:5px;'></td>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa;padding:5px;'></td>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa;padding:5px;'></td>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa;padding:5px;'>Total</td>";
		$out.="<td  style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;' align='right'>{$currecnySyb}{$orderTotal}</td>";	
		$out.="</tr>";
		
		$out.="</table>";	
		
		$out.="<page_footer>";
		
		$out.="<br><table style='width: 100%;'>";
		$out.="<tr>";
		$out.='<td style="width:100%;background-color:#000;padding:0px 20px 0px;font-family:Helvetica,Arial,sans-serif;font-size:12px;color:#fff;text-align:left;vertical-align:middle;height:41px">&copy; Where its at '.date('Y').'. All Rights Reserved</td>';
		$out.=" </tr>";
		$out.="</table>";	
		
		$out.="</page_footer>";
			
		$out.="</page>";	
		
	}else{
		$out.="<div class='invalid-order error messages'>invalid order.</div>";
	}
	//echo $out;die;
	$fileName="wia_order_no_{$orderId}.pdf";
	
    // convert in PDF
    require_once(libraries_get_path('html2pdf').'/html2pdf.class.php');
    try
    {
       	$html2pdf = new HTML2PDF('P', 'A4', 'fr', true, 'UTF-8', 3);
		//$html2pdf->setModeDebug();
		$html2pdf->pdf->SetDisplayMode('fullpage');
        $html2pdf->setDefaultFont('Helvetica');
        $html2pdf->writeHTML($out);
        $html2pdf->Output($fileName);
    }
    catch(HTML2PDF_exception $e) {
        echo $e;
        exit;
    }
	die;
}






/*****
#action: admin_booking_booking_details
#description: show booking details
#author: PKS
#created: 30 July 2015
*****/
function admin_booking_booking_details($orderId=null){
	
	global $base_url;
	
	if(empty($orderId))
	$orderId=arg(2);
	
	if(empty($orderId)){return "<div class='invalid-order error messages'>invalid order.</div>";}
	
	$order=commerce_order_load($orderId);
	$out='';	
	
	if($order){			
		
		$orderStatus=$order->status;
		$orderDate=date("d/m/Y",$order->created);
		$orderTotal=commerce_currency_format($order->commerce_order_total['und'][0]['amount']);
		
		$vatTotal=commerce_currency_format($order->commerce_order_total['und'][0]['data']['components'][1]['price']['amount']);
		
		$currecnySyb=CURRENCY_SYMBOL;
		
		$lineItems=$order->commerce_line_items;
		
		
		$bilingInfo=commerce_customer_profile_load($order->commerce_customer_billing['und'][0]['profile_id']);
		$address='';
		if($bilingInfo){
			
			if($bilingInfo->commerce_customer_address['und'][0]['name_line'])
			$address.=$bilingInfo->commerce_customer_address['und'][0]['name_line'].", ";
			
			if($bilingInfo->commerce_customer_address['und'][0]['locality'])
			$address.=$bilingInfo->commerce_customer_address['und'][0]['locality'].", ";
			
			if($bilingInfo->commerce_customer_address['und'][0]['postal_code'])
			$address.="Pin - ".$bilingInfo->commerce_customer_address['und'][0]['postal_code'].", ";
			
			$address=trim($address,", ");
		}
	
		
		$out.="<div id='order_detail_block'>";
		$out.="<div class='cart-table1'>";
		
		$out.="<h2>Order Detail: #{$orderId}</h2>";	
		$out.="<table width='100%' border='1' cellspacing='0' cellpadding='0' class='cart-table'>";	  
		
		$out.="<tr>";
		$out.="<th>Order No.</th>";	
		$out.="<th>Description</th>";	
		$out.="<th>Date of Booking</th>";
		$out.="<th>Status</td>";
		$out.="<th align='right'>Total Amount</th>";
		$out.="<th>&nbsp;</th>";
		$out.="</tr>";
		
		$out.="<tr class='odd'>";
		$out.="<td>#{$orderId}</td>";
		$out.="<td>{$address}</td>";
		$out.="<td>{$orderDate}</td>";
		$out.="<td>{$orderStatus}</td>";
		$out.="<td align='right'>{$currecnySyb}{$orderTotal}</td>";
		$out.="<td align='right'>&nbsp;</td>";
		$out.="</tr>";
		$out.="</tbody>";
				
		$out.="<tr>";
		$out.="<td style='border:none' colspan='6'>&nbsp;</td>";	
		$out.="</tr>";
	
		$out.="<tr>";
		$out.="<td style='border:none' colspan='6'>&nbsp;</td>";	
		$out.="</tr>";
		
		$out.="<tr class='odd'>";
		$out.="<th width='10%'>Image</th>";
		$out.="<th width='30%'>Venue</th>";
		$out.="<th width='20%'>Time Slots</th>";
		$out.="<th width='20%'>Amenities</th>";
		$out.="<th align='right'>Amount</th>";
		$out.="<th>&nbsp;</th>";
		$out.="</tr>";
		$out.="</thead>";
		$out.="<tbody>";
		
		if($lineItems){
			foreach($lineItems['und'] as $_lineItem){
				
				$line_item_id=$_lineItem['line_item_id'];		
				
				$rowItem=commerce_line_item_load($line_item_id);			
				$pwrapper = entity_metadata_wrapper('commerce_product', $rowItem->commerce_product['und'][0]['product_id']);		
				
				$itemPrice=commerce_currency_format($rowItem->commerce_unit_price['und'][0]['amount']);
				$itemPriceDisplay=CURRENCY_SYMBOL." ".$itemPrice;		
				$slotDec=$rowItem->data['slot_details']['slot_descriptions'];
				
				$itemTitle=$pwrapper->title->value();
				$roomName=$pwrapper->field_room_name->value();
				
				$out.="<tr  class='even'>";        
				$out.="<td>";
				
				$fImg = $pwrapper->field_featured_image->value();
				 
				if($fImg && $fImg['uri']){
					
					$img = array(
					'style_name' => 'cart_thumbnail',
					'path' => $fImg['uri'],
					'alt' => $fImg['alt'],
					'title' => $fImg['title']			
					);
				
					$out.=theme('image_style',$img);
				}else{
					$out.='<img src="'.$base_url.'/'.$theme_path.'/images/no-image.jpg" width="50"/>';
				}                
				
			
				$out.="</td>";
				$out.="<td><strong>{$itemTitle}";
				$out.="&nbsp;({$roomName})</strong>";
				
				
				if($pwrapper->field_full_address->value()){
					$out.="<br/>".$pwrapper->field_full_address->value();
				}              
				
				$out.="</a></strong></td>";
				$out.="<td>{$slotDec}</td>";

				$amenities = $rowItem->data['req_data']['amenities'];
				$ame=get_amenites_by_aids(explode(",",$amenities),'list');
						
				$out.="<td>{$ame}</td>";
			
				$out.="<td align='right'>{$currecnySyb}{$itemPrice}</td>";	
				$out.="<td align='right'>&nbsp;</td>";				
				$out.="</tr>";
				
			}
		}	
	
		
		$out.="<tr class='grand-total odd'>";
		$out.="<td class='remove'></td>";
		$out.="<td class='remove'></td>";
		$out.="<td class='remove'></td>";
		$out.="<td>vat</td>";
		$out.="<td align='right'>{$currecnySyb}{$vatTotal}</td>";	
		$out.="<td align='right'>&nbsp;</td>";
		$out.="</tr>";
		
		$out.="<tr class='grand-total even'>";
		$out.="<td class='remove'></td>";
		$out.="<td class='remove'></td>";
		$out.="<td class='remove'></td>";
		$out.="<td>total</td>";
		$out.="<td align='right'>{$currecnySyb}{$orderTotal}</td>";	
		$out.="<td align='right'>&nbsp;</td>";
		$out.="</tr>";
		
		$out.="</table>";	
		
		$out.="</div>";		
		$out.="</div>";	
		
		$out.="<div class='cart-btn'>";	
		$out.="<a href='".$base_url."/admin/commerce/orders' class='btn left'>back</a>";
		$out.="</div>";
		
	}else{
		$out.="<div class='invalid-order error messages'>invalid order.</div>";
	}
	
		
	return $out;

}
