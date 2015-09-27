<div class="row">
<?php
global $base_url;
$orderId=$form['order_id']['#value'];
$order=commerce_order_load($orderId);
	
if($order && strtolower($order->status)=='completed'){	

	$currecnySyb=CURRENCY_SYMBOL;
	
	$orderStatus=$order->status;
	$orderDate=date("d/m/Y",$order->created);
	$orderTotal=commerce_currency_format($order->commerce_order_total['und'][0]['amount']);	
	$vatTotal=commerce_currency_format($order->commerce_order_total['und'][0]['data']['components'][1]['price']['amount']);	
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
	?>
	<div id='order_detail_block'>
	<h2>Order Detail: #<?php echo $orderId;?></h2>	
	<table width='100%' border='0' cellspacing='0' cellpadding='0' class='cart-table'>	  
	<thead>
	<tr>
	<th>Order No.</th>	
	<th>Description</th>	
	<th>Date of Booking</th>
	<th>Status</th>
	<th align='right'>Total Amount</th>
	<th>&nbsp;</th>
	</tr>
	</thead>
	<tbody>
	<tr class='odd'>
	<td>#<?php echo $orderId;?></td>
	<td><?php echo $address;?></td>
	<td><?php echo $orderDate;?></td>
	<td><?php echo $orderStatus;?></td>
	<td align='right'><?php echo $currecnySyb.$orderTotal;?></td>
	<td align='right'>
	</td>
	</tr>
	</tbody>
	</table>		
			
	<div class='cancel-booking-slots'>	
    <h2>Cancel Booking(s)</h2>		
	<table width='100%' border='0' cellspacing='0' cellpadding='0' class='cart-table'>
	<thead>
	<tr>
	<th>Slot</th>	
	<th>Status</th>
	</thead>
	<tbody>
	<?php
	if($lineItems){
		foreach($lineItems['und'] as $_lineItem){
			
			$lineItemId=$_lineItem['line_item_id'];	
			
			$lineItem=commerce_line_item_load($lineItemId);			
			$pwrapper = entity_metadata_wrapper('commerce_product', $lineItem->commerce_product['und'][0]['product_id']);
			$itemTitle=$pwrapper->title->value();
						
			$rowHead='';		
			$rs=get_slots_by_order($orderId, $lineItemId,$order->uid);	
			
			if($rs){
				
				foreach($rs as $_slot){
					
					$dateTS=strtotime($_slot->date);
					$date=date("d/m/Y",$dateTS);
					
					if($rowHead!=$dateTS){
						?>
						<tr class='even'><td colspan='2'>
                        <div class="margintop20 paddingbottom20"></div>
                        <div class='center slot-desc'>                       
                        <strong>date&nbsp;:&nbsp;</strong><?php echo $date;?>
						&nbsp;&nbsp;<strong>venue&nbsp;:&nbsp;</strong><?php echo $itemTitle;?></div>          
                           
                        </td></tr>
						<?php
						$rowHead=$dateTS;							
					}
					?>
					<tr>
					<td><?php print render($form['time_slot'][$orderId][$lineItemId][$_slot->id]);?></td>
					<td><?php echo $_slot->status;?></td>
					</tr>
					<?php				
					}
				}          
			
			}
		
	}?>
	</tbody>
	</table>	
	<?php
	print render($form['order_id']);
	print render($form['form_id']);
	print render($form['form_build_id']);
	print render($form['form_token']);
	?>
    <?php print render($form['refund_type']);?>
    <?php print render($form['submit']);?>	
    <a class="admin-back btn" href="<?php echo $base_url;?>/admin/commerce/orders">Back</a>&nbsp;
    <a class="admin-refund btn" href="<?php echo $base_url;?>/admin/commerce/orders/<?php echo $orderId;?>/edit">Go to Refund</a>
      	
	</div>		
	</div>
	
<?php
	
}else{
	echo "<div class='invalid-order error messages'>Invalid order.</div>";
}
?>
</div>