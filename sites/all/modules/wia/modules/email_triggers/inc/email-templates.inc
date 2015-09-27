<?php
/*****
#action: send_email
#description: send email
#author: PKS
#created: 14 Aug 2015
*****/
function send_email($mailto,$subject=null,$message=null,$mailfrom=null){
	
	if(!empty($mailto)){
		
		if(!empty($mailfrom))
		$mailfrom = variable_get('site_mail', ini_get('sendmail_from'));
			
		if(drupal_mail('email_triggers', 'reply', $mailto, language_default(), array('body' => $message, 'subject' => $subject), $mailfrom, TRUE)){
			return true;
		}
	}
	return false;
}



/*****
#action: send_booking_email
#description: send booking email
#author: PKS
#created: 14 Aug 2015
*****/

function send_booking_email($order){	
			
	if($order){
		
		$orderStatus='processing';
		if($order->status=='checkout_complete'){
			$orderStatus='complete';
		}
		
		$orderDate=date("d/m/Y",$order->created);
		$orderTotal=commerce_currency_format($order->commerce_order_total['und'][0]['amount']);
		
		$vatTotal=commerce_currency_format($order->commerce_order_total['und'][0]['data']['components'][1]['price']['amount']);
		
		$currecnySyb=CURRENCY_SYMBOL;
		
		$lineItems=$order->commerce_line_items;
		$orderNbr=$order->order_number;
		
		
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
	
		$out='';
		
		
		$out.="<table style='width:100%;' cellspacing='0' cellpadding='2'>";	
		$out.="<tr><td style='padding: 45px 20px 10px; font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;'>";
		$out.="<p style='margin:0px 20px 10px;'>Thank for booking with whereitsat.</p>";		
		$out.="<p style='margin:0px 20px 10px;'>Your booking reference is: #{$orderNbr}. And booking details are:</p>";	
		$out.="</td></tr>";	
	
		$out.="<tr><td style='padding: 45px 20px 10px; font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;'>";		
		
		$out.="<table border='0'  cellspacing='0' cellpadding='2' style='margin:0px 20px 10px;border-top:solid 1px #aaaaaa; border-left:solid 1px #aaaaaa;'>";	  
		
		$out.="<tr>";
		$out.="<td width='20%' style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'><strong>order no.</strong></td>";	
		$out.="<td width='30%' style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'><strong>description</strong></td>";	
		$out.="<td width='20%' style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'><strong>date of booking</strong></td>";
		$out.="<td width='10%' style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'><strong>status</strong></td>";
		$out.="<td width='20%' style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'><strong>total amount</strong></td>";
		$out.="</tr>";
		
		
		$out.="<tr>";
		$out.="<td width='20%' style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'>#{$orderNbr}</td>";
		$out.="<td width='30%' style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'>{$address}</td>";
		$out.="<td width='20%' style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'>{$orderDate}</td>";
		$out.="<td width='10%' style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'>{$orderStatus}</td>";
		$out.="<td width='20%' style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'>{$currecnySyb}{$orderTotal}</td>";
		$out.="</tr>";	
		$out.="</table>";		
		
		$out.="</td></tr>";		
		
		
		$out.="<tr><td style='padding: 45px 20px 10px; font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555'>";				
	
		
		$out.="<table border='0'  cellspacing='0' cellpadding='2' style='margin:0px 20px 10px;border-top:solid 1px #aaaaaa; border-left:solid 1px #aaaaaa;'>";
		$out.="<tr>";
		$out.="<td width='10%' style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'><strong>image</strong></td>";
		$out.="<td width='30%' style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'><strong>venue</strong></td>";
		$out.="<td width='20%' style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'><strong>time Slots</strong></td>";
		$out.="<td width='20%' style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'><strong>amenities</strong></td>";
		$out.="<td width='10%' style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'><strong>amount</strong></td>";
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
				
				$out.="<tr>";        
				$out.="<td  width='10%' style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'>";
				
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
				$out.="<td width='30%' style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'>{$itemTitle}";
				
				if($pwrapper->field_full_address->value()){
					$out.="<br/>".$pwrapper->field_full_address->value();
				}              
				
				$out.="</td>";
				$out.="<td width='20%' style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'>{$slotDec}</td>";

				$amenities = $rowItem->data['req_data']['amenities'];
				$ame=get_amenites_by_aids(explode(",",$amenities),'list');
						
				$out.="<td width='20%' style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'>{$ame}</td>";
			
				$out.="<td width='10%' style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'>{$currecnySyb}{$itemPrice}</td>";					
				$out.="</tr>";
				
			}
		}		
		
		$out.="<tr style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555'>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa;padding:5px;'>&nbsp;</td>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa;padding:5px;'>&nbsp;</td>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa;padding:5px;'>&nbsp;</td>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa;padding:5px;'>vat</td>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'>{$currecnySyb}{$vatTotal}</td>";	
		$out.="</tr>";
		
		$out.="<tr>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa;padding:5px;'>&nbsp;</td>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa;padding:5px;'>&nbsp;</td>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa;padding:5px;'>&nbsp;</td>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa;padding:5px;'>total</td>";
		$out.="<td style='font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555;border-bottom:solid 1px #aaaaaa; border-right:solid 1px #aaaaaa;padding:5px;'>{$currecnySyb}{$orderTotal}</td>";	
		$out.="</tr>";		
		$out.="</table>";
		
		$out.="</td></tr>";			
		
		$out.="<tr><td style='padding: 45px 20px 10px; font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555'>";		
	
		$out.="<p style='margin:0px 20px 10px;'>kind regards</p>";
		$out.="<p style='margin:0px 20px 10px;'>the whereitsat team</p>";	
		
		$out.="</td></tr>";		
		$out.="</table>";	
		
		$toEmail=$order->mail;
		
		$sub="Welcome to The Where its at - Room Booking";		
		if(send_email($toEmail,$sub,$out))
		return true;
			
	}
	return false;
	
}


/*****
#action: send_booking_cancellation_email
#description: send booking cancellation email
#author: PKS
#created: 25 Aug 2015
*****/

function send_booking_cancellation_email($order,$canceledData){	
			
	if($order){		
	
		$orderNbr=$order->order_number;
		$currecnySyb=CURRENCY_SYMBOL;		
		$amoutToRefund=$canceledData['amount'];
		$noRefundHours=0;		
		$noRefundPolicy=get_refund_rule_by_deduction(100);
		$noRefundHourLimit=$noRefundPolicy->to_hours;	
		
		$out='';
		
		$out.="<table style='width:100%' cellspacing='0' cellpadding='2'>";	
		$out.="<tr><td style='padding: 45px 20px 10px; font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555'>";
		
		
		if($amoutToRefund>0){
			$out.="<p style='margin:0px 0px 10px;'>Thank for cancellation request for booking reference #{$orderNbr}.</p>";
			$out.="<p style='margin:0px 0px 10px;'>This has now been actioned. {$currecnySyb}{$amoutToRefund} has now been refunded to the debit/credit card used for the original booking.</p>";
		}else{
			
			
			$out.="<p style='margin:0px 0px 10px;'>Thank for cancellation request for booking reference #{$orderNbr}.</p>";
			$out.="<p style='margin:0px 0px 10px;'>Unfortunately, cancellations must be made a minimum of {$noRefundHourLimit} hours in advance so on the occasion as we have received less than {$noRefundHourLimit} hours no refund can be given.</p>";
		}
		
		$out.="</tr>";	
		
		$out.="<tr><td style='padding: 45px 20px 10px; font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555'>";		
	
		$out.="<p style='margin:0px 0px 10px;'>kind regards</p>";
		$out.="<p style='margin:0px 0px 10px;'>the whereitsat team</p>";	
		
		$out.="</td></tr>";	
		$out.="</table>";
		
		$toEmail=$order->mail;
		
		$sub="Welcome to The Where its at - Room Cancellation by User";		
		if(send_email($toEmail,$sub,$out))
		return true;
			
	}
	return false;
	
}



/*****
#action: send_admin_booking_cancellation_email
#description: send admin booking cancellation email
#author: PKS
#created: 25 Aug 2015
*****/

function send_admin_booking_cancellation_email($order,$canceledData){	
			
	if($order){		
	
		$orderNbr=$order->order_number;
		$currecnySyb=CURRENCY_SYMBOL;		
		$amoutToRefund=$canceledData['amount'];
		$noRefundHours=0;		
		$noRefundPolicy=get_refund_rule_by_deduction(100);
		$noRefundHourLimit=$noRefundPolicy->to_hours;
		
		$refundType=$canceledData['refund_type'];
		
		$out='';
		
		$out.="<table style='width:100%' cellspacing='0' cellpadding='2'>";	
		$out.="<tr><td style='padding: 45px 20px 10px; font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555'>";
		
		
		if($refundType=='fr'){
			$out.="<p style='margin:0px 20px 10px;'>I write with reference to you booking, reference #{$orderNbr}.</p>";
			$out.="<p style='margin:0px 0px 10px;'>Unfortunately, for reasons outside our control, the facility is no longer available. We have made a provisional alternative booking at the nearest equivalent facility which you may revew on the website www.whereitsat.today/input.</p>";			
			$out.="A full refund will be made to the  debit/credit card used for the original booking. We do apologise for this inconvenience which falls below the standard we set for ourselves and our facilities and which is beyond our control";
		}else{			
			
			$out.="<p style='margin:0px 0px 10px;'>I write with reference to you booking, reference #{$orderNbr}.</p>";
			$out.="<p style='margin:0px 0px 10px;'>Unfortunately, for reasons outside our control, the facility is no longer available. We have made a provisional alternative booking at the nearest equivalent facility which you may revew on the website www.whereitsat.today/input.</p>";
			
			$out.="<p style='margin:0px 0px 10px;'>A refund of {$currecnySyb}{$amoutToRefund} will be made to the  debit/credit card used for the original booking. We do apologise for this inconvenience which falls below the standard we set for ourselves and our facilities and which is beyond our control</p>";
		}
		
		$out.="</tr>";	
		
		$out.="<tr><td style='padding: 45px 20px 10px; font-family:Helvetica,Arial,sans-serif;font-size:13px;line-height:21px;color:#555555'>";		
	
		$out.="<p style='margin:0px 0px 10px;'>kind regards</p>";
		$out.="<p style='margin:0px 0px 10px;'>the whereitsat team</p>";	
		
		$out.="</td></tr>";	
		$out.="</table>";
		
		$toEmail=$order->mail;
		
		$sub="Welcome to The Where its at - Room Cancellation by Admin";		
		if(send_email($toEmail,$sub,$out))
		return true;
			
	}
	return false;
	
}

