<?php

/**
 * Implements base_settings_form()
 */
function booking_pane_settings_form($checkout_pane) {
	return array();
}

/**
 * Implements base_checkout_form()
 */
function booking_pane_checkout_form($form, $form_state, $checkout_pane, $order) {
		
	global $base_url;
	$orderId=$order->order_number;
	
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

	
	$out="<div id='order_detail_block'>";
	$out.="<div class='cart-table1'>";
	
	$out.="<h1>order detail: #{$orderId}</h1>";	
	$out.="<table width='100%' border='0' cellspacing='0' cellpadding='0' class='cart-table'>";	  

	$out.="<tr>";
	$out.="<td>order no.</td>";	
	$out.="<td>description</td>";	
	$out.="<td>date of booking</td>";
	$out.="<td>status</td>";
	$out.="<td align='right'>total amount</td>";
	$out.="<td>&nbsp;</td>";
	$out.="</tr>";

	$out.="<tr>";
	$out.="<td><span class='label-mob'>order no.</span>#{$orderId}</td>";
	$out.="<td><span class='label-mob'>description</span>{$address}</td>";
	$out.="<td><span class='label-mob'>date of booking</span>{$orderDate}</td>";
	$out.="<td class='lowercase'><span class='label-mob'>status</span>{$orderStatus}</td>";
	$out.="<td align='right'> <span class='label-mob'>total amount</span>{$currecnySyb}{$orderTotal}</td>";
	$out.="<td align='right'>&nbsp;</td>";
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
			$itemTitle=$pwrapper->title->value();
			$roomName=$pwrapper->field_room_name->value();
					
			
			$itemPrice=commerce_currency_format($rowItem->commerce_unit_price['und'][0]['amount']);
			$itemPriceDisplay=CURRENCY_SYMBOL." ".$itemPrice;		
			$slotDec=$rowItem->data['slot_details']['slot_descriptions'];
			
			
			
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
			$out.="<td align='right'>&nbsp;</td>";				
	   		$out.="</tr>";
			
		}
	}	


	$out.="<tr class='grand-total'>";
	$out.="<td class='remove'></td>";
	$out.="<td class='remove'></td>";
	$out.="<td class='remove'></td>";
	$out.="<td>vat</td>";
	$out.="<td align='right'>{$currecnySyb}{$vatTotal}</td>";
	$out.="<td align='right'>&nbsp;</td>";	
	$out.="</tr>";
	
	$out.="<tr class='grand-total'>";
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
	$out.="<a href='{$base_url}/download-booking-details/{$orderId}/download_pdf' class='btn1 left' target='_blank' style='margin-right:10px'>download pdf</a>";
	$out.="<a href='#' class='btn1 left' onclick=\"print_block('order_detail_block')\" style='margin-right:10px'>print order</a>";
	//$out.="<a href='#' class='btn left'>download PDF</a>";
	$out.="</div>";
		
	
	$checkout_form['order_details'] = array(
		'#markup' => $out,
	);
	
	return $checkout_form;
}