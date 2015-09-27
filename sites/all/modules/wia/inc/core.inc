<?php
/*****
#action: format_bookings
#description: format row date to array format.
#author: PKS
#created: 08 June 2015
*****/
function format_bookings($rs) {
	
	$arrRes=array();
	if($rs){
		foreach($rs as $row){
			$arrRes[$row->date][$row->product_id][$row->slot_id]=array('status'=>$row->status,'data'=>$row);
		}
	}	
	return $arrRes;	
}


/*****
#action: get_amenities
#description: get all amenities as either raw or list data
#author: PKS
#created: 11 June 2015
*****/
function get_amenities($resType='raw'){//raw: returns objects data, list: returns list data contaning id as key and name as value

	$name = 'amenities';
	$tax = taxonomy_vocabulary_machine_name_load($name);
	$rs = taxonomy_get_tree($tax->vid);	
	
	
	if($rs && $resType=='list'){
		
		$listData=array();
		
		foreach($rs as $amen){
			$listData[$amen->tid]=$amen->name;
		}
		
		return $listData;
	}
	
	return $rs;
	
}


/*****
#action: get_locations
#description: get all locations as either raw or list data
#author: PKS
#created: 11 June 2015
*****/
function get_locations($resType='raw'){//raw: returns objects data, list: returns list data contaning id as key and name as value

	$name = 'locations';
	$tax = taxonomy_vocabulary_machine_name_load($name);
	$rs = taxonomy_get_tree($tax->vid);	
	
	if($rs && $resType=='list'){
		
		$listData=array();
			
		foreach($rs as $loc){
			$listData[$loc->tid]=$loc->name;
		}
		
		return $listData;
	}
	
	return $rs;
	
}

/*****
#action: get_space_types
#description: get all space types as either raw or list data
#author: PKS
#created: 11 June 2015
*****/
function get_space_types($resType='raw'){//raw: returns objects data, list: returns list data contaning id as key and name as value

	$name = 'space_types';
	$tax = taxonomy_vocabulary_machine_name_load($name);
	$rs = taxonomy_get_tree($tax->vid);	
	
	if($rs && $resType=='list'){
		
		$listData=array();
	
		foreach($rs as $spt){
			$listData[$spt->tid]=strtolower($spt->name);
		}
		
		return $listData;
	}
	
	return $rs;
	
}


/*****
#action: get_time_slots
#description: get all space types as either raw or list data
#author: PKS
#created: 11 June 2015
*****/
function get_time_slots($resType='raw'){//raw: returns objects data, list: returns list data contaning id as key and name as value

	$name = 'time_slots';
	$tax = taxonomy_vocabulary_machine_name_load($name);
	$rs = taxonomy_get_tree($tax->vid);	
	
	if($rs && $resType=='count'){
			
		return count($rs);
	}	
	
	if($rs && $resType=='list'){
		$listData=array();	
		foreach($rs as $loc){
			$listData[$loc->tid]=$loc->name;
		}
		
		return $listData;
	}	
	return $rs;
	
}


/*****
#action: get_price_range
#description: get all space types as either raw or list data
#author: PKS
#created: 11 June 2015
*****/
function get_price_range(){//return min and max price range
	
	$arrRes=array('min_price'=>0,'max_price'=>0);

	$rs=db_select('slot_prices','sp')
	->fields('sp');
	
	$rs->addExpression('MAX(sp.amount)','max_price');
	$rs->addExpression('MIN(sp.amount)','min_price');
	
	$_rs=$rs->execute();
	$rs=$_rs->fetchAll();
	
	if($rs){
		$arrRes['min_price']=$rs[0]->min_price;
		$arrRes['max_price']=$rs[0]->max_price;
	}
	
	return $arrRes;
	
}


/*****
#action: get_gmap_center
#description: get google map center among set of latitude and longitude
#author: PKS
#created: 17 June 2015
*****/
function get_gmap_center($arrData){
	
	$minlat = false;
	$minlng = false;
	$maxlat = false;
	$maxlng = false;
	
	foreach ($arrData as $data_coords) {
	
		if (isset($data_coords['longitude'])) {
			
			if ($minlat === false) { 
				$minlat = $data_coords['latitude']; 
			} else {
				 $minlat = ($data_coords['latitude'] < $minlat) ? $data_coords['latitude'] : $minlat; 
			}
			
			if ($maxlat === false) {
				 $maxlat = $data_coords['latitude']; 
			} else {
				 $maxlat = ($data_coords['latitude'] > $maxlat) ? $data_coords['latitude'] : $maxlat;
			}
			
			if ($minlng === false) {
				 $minlng = $data_coords['longitude']; 
			} else {
				 $minlng = ($data_coords['longitude'] < $minlng) ? $data_coords['longitude'] : $minlng;
			}
			
			if ($maxlng === false) {
				 $maxlng = $data_coords['longitude']; 
			} else {
				 $maxlng = ($data_coords['longitude'] > $maxlng) ? $data_coords['longitude'] : $maxlng;
			}
		}
	}
	$lat = $maxlat - (($maxlat - $minlat) / 2);
	$lng = $maxlng - (($maxlng - $minlng) / 2);
	
	return array('latitude'=>$lat,'longitude'=>$lng);
}


/*****
#action: convet_date_tosys_format
#description: get google map center among set of latitude and longitude
#author: PKS
#created: 17 June 2015
*****/

function convet_date_tosys_format($date,$dateFormat="d/m/y"){
	
	$newDate=false;
	
	if(!empty($date)){
	
		switch($dateFormat){
			
			case "d/m/y" : 
			
				$arrDate=explode("/",$date);
				$newDate=$arrDate[2]."-".$arrDate[1]."-".$arrDate[0];
				break;
			
			case "m/d/y" : 
			
				$arrDate=explode("/",$date);
				$newDate=$arrDate[2]."-".$arrDate[0]."-".$arrDate[1];
				break;
			
			case "y/m/d" :
			
				$arrDate=explode("/",$date);
				$newDate=$arrDate[0]."-".$arrDate[1]."-".$arrDate[2];
				break;
			
			
			default :
				$newDate=$date;
				break;
			
			
		}
	}
	
	return $newDate;
	
}


/*****
#action: get_amenity_prices
#description: get amenity prices by product id.
#author: PKS
#created: 02 June 2015
*****/
function get_amenity_prices($productId,$format=true){
	
	$rs = db_select('amenity_prices', 'amenity')
	->fields('amenity')
	->condition('amenity.product_id', $productId, '=')
	->execute()
	->fetchAll();
	
	if($rs && $format){
		$arrData=array();
		foreach($rs as $row){
			$arrData[$row->amenity_id]=array('amenity_status'=>$row->amenity_status,'price'=>$row->amount);
		}		
		return $arrData;
	
	}else{
		return $rs;
	}
	
}



/*****
#action: get_amenites_price_by_aid_and_pid
#description: get amenity total prices by amenites and product id.
#author: PKS
#created: 02 June 2015
*****/
function get_amenites_price_by_aids_and_pid($amenityIds,$productId){
	
	$rs = db_select('amenity_prices', 'amenity')
	->fields('amenity')
	->condition('amenity.product_id', $productId, '=')
	->condition('amenity.amenity_id', $amenityIds, 'IN')
	->execute()
	->fetchAll();	
	
	$totalPrice=0;

	if($rs){		
		foreach($rs as $row){
			$totalPrice+=$row->amount;
		}
	
	}	
	return $totalPrice;	
}

/*****
#action: get_amenites_by_aid_and_pid
#description: get amenity by amenites and product id.
#author: PKS
#created: 02 June 2015
*****/
function get_amenites_by_aids($amenityIds,$format=NULL){
	
	$rs = taxonomy_term_load_multiple($amenityIds);
	
	$out=$rs;
	if($format && $format=='list'){
	
		$out="<ul class='ame_list lowercase' style='list-style:none;'>";		
		if($rs){
			foreach($rs as $_row){
				$out.="<li  style='list-style:none;'>".$_row->name."</li>";
			}
		}else{
			$out.="<li style='list-style:none;'>n/a</li>";
		}
		$out.="</ul>";
	
	}
	
	return $out;	
}






/*****
#action: get_slot_prices
#description: get slot prices by product id.
#author: PKS
#created: 02 June 2015
*****/
function get_slot_prices($productId,$format=true){
	
	$rs = db_select('slot_prices', 'slot')
	->fields('slot')
	->condition('slot.product_id', $productId, '=')
	->execute()
	->fetchAll();
	
	if($rs && $format){
		$arrData=array();
		foreach($rs as $row){
			$arrData[$row->slot_id]=array('slot_status'=>$row->slot_status,'price'=>$row->amount);
		}		
		return $arrData;
	}else{
		return $rs;
	}

}


/*****
#action: get_amenities_by_product_id
#description: get product specific amenities.
#author: PKS
#created: 29 June 2015
*****/
function get_amenities_by_product_id($productId){
	
	$rs = db_select('amenity_prices', 'amenity')
	->fields('amenity')
	->condition('amenity.product_id', $productId, '=')
	->condition('amenity.amenity_status', 1, '=')
	->execute()
	->fetchAll();	
	
	$tids=null;	
	if($rs){
		foreach($rs as $row){
			$tids[]=$row->amenity_id;
		}	
	}
	
	return taxonomy_term_load_multiple($tids);
	
	
}

/*****
#action: get_slots_by_product_id
#description: get product specific slots.
#author: PKS
#created: 29 June 2015
*****/
function get_slots_by_product_id($productId){
	
	$rs = db_select('slot_prices', 'slot')
	->fields('slot')
	->condition('slot.product_id', $productId, '=')
	->condition('slot.slot_status', 1, '=')
	->execute()
	->fetchAll();
	
	$tids=null;	
	if($rs){
		foreach($rs as $row){
			$tids[]=$row->slot_id;
		}	
	}	
	return taxonomy_term_load_multiple($tids);	
	
}


/*****
#action: get_slot_details_by_id
#description: get slot details by slot term id.
#author: PKS
#created: 29 June 2015
*****/
function get_slot_details_by_id($termId){	
	
	return taxonomy_term_load($termId);		
}


/*****
#action: get_referencing_node_id
#description: get corresponding node id of a product.
#author: PKS
#created: 29 June 2015
*****/

function get_referencing_node_id($productId) {
	//Itterate through all fields which refer to commerce products.
	foreach (commerce_info_fields('commerce_product_reference') as $field) {
		// Build query.
		$query = new EntityFieldQuery;
		$query->entityCondition('entity_type', 'node', '=')
		->fieldCondition($field['field_name'], 'product_id', $productId, '=')
		->range(0, 1);
		
		if ($result = $query->execute()) {
			// Return node id.
			return @array_shift(array_keys($result['node']));
		}
	} 
	return false;
}

/*****
#action: get_slots_by_product_id
#description: get product lat and long.
#author: PKS
#created: 29 July 2015
*****/
function get_latlong_by_product_ids($productIds, $format=true){
	
	//print_r($productIds);
	$rs = db_select('asset_latlongs', 'latlongs')
	->fields('latlongs')
	->condition('latlongs.product_id', $productIds, 'IN')	
	->execute()
	->fetchAll();
	
	$latlong=null;	
	if($rs && $format){
		foreach($rs as $row){
			
			$latlong[$row->product_id]=array('latitude'=>$row->latitude,'longitude'=>$row->longitude,'title'=>$row->address);
		}	
	
		return $latlong;
	}else{
		return $rs;;
	}
}



/*****
#action: get_slots_by_order
#description: get all booking details by order id and line item 
#author: PKS
#created: 29 June 2015
*****/
function get_slots_by_order($orderId, $lineItemId, $userId, $status=NULL){
	
	if(empty($status))
	{
		$status=array('Booked','Unavailable','Available','Processing','Canceled');
	}

	$query = db_select('slot_bookings', 'sb');	
	$query->leftJoin('taxonomy_term_data', 'td', 'sb.slot_id = td.tid')	;	
	
	$rs=$query->fields('sb')
	->fields('td',array('tid','name'))
	->condition('sb.order_id', $orderId, '=')
	->condition('sb.line_item_id', $lineItemId, '=')
	->condition('sb.uid', $userId, '=')
	->condition('sb.status', $status , 'IN')	
	->orderBy('sb.date','ASC')
	->orderBy('sb.slot_id','ASC')
	->execute()
	->fetchAll();
		
	return $rs;	
	
}


/*****
#action: get_slot_detail_by_booking_id
#description: get slot details by booking id 
#author: PKS
#created: 29 June 2015
*****/
function get_slot_detail_by_booking_id($bookingId, $orderId, $lineItemId, $userId){
	
	$query = db_select('slot_bookings', 'sb');	
	$query->fields('sb')	
		->condition('sb.order_id', $orderId, '=')
		->condition('sb.line_item_id', $lineItemId, '=')
		->condition('sb.uid', $userId, '=')
		->condition('sb.id',$bookingId, '=');
	
	$rs=$query->execute()
		->fetch();
		
	if($rs){
		
		//load slot details
		$slotMeta=null;
		if($rs->slot_id){
			$slotMeta=taxonomy_term_load($rs->slot_id);			
		}
		 
	}
		
	return array('slot_booking'=>$rs,'slot_meta'=>$slotMeta);	
	
}




/*****
#action: get_slot_ids_by_order
#description: get booking slot ids by order
#author: PKS
#created: 29 June 2015
*****/
function get_slot_booking_ids_by_order($orderId, $lineItemId, $userId, $status=NULL){
	
	if(empty($status))
	{
		$status=array('Booked','Unavailable','Available','Processing');
	}

	$query = db_select('slot_bookings', 'sb');		
	$rs=$query->fields('sb',array('id'))
	->condition('sb.order_id', $orderId, '=')
	->condition('sb.line_item_id', $lineItemId, '=')
	->condition('sb.uid', $userId, '=')
	->condition('sb.status', $status , 'IN')	
	->orderBy('sb.date','ASC')
	->orderBy('sb.slot_id','ASC')
	->execute()
	->fetchAll();
	
	$arrRes=array();
	
	if($rs){
		foreach($rs as $_row){
			$arrRes[]=$_row->id;
		}	
	}
	
	return $arrRes;	
	
}

/*****
#action: get_refund_rules
#description: get refund policy data
#author: PKS
#created: 7 Aug 2015
*****/
function get_refund_rules($status=null,$format=true){//if status is not passed, will returns all results
	
	$query = db_select('refund_policy', 'rp');	
	$query->fields('rp');
	
	if($status){	
		$query->condition('rp.status', $status, '=');		
	}
	
	$rs=$query->orderBy('rp.policy_nbr','ASC')
		->execute()
		->fetchAll();
	
	if($rs && $format){
		
		$arrRes=array();
		foreach($rs as $_row){
			
			$arrRes[$_row->policy_nbr]=array('from_hours'=>$_row->from_hours,
			'to_hours'=>$_row->to_hours,
			'amount'=>$_row->amount,
			'status'=>$_row->status);
		}
				
		return $arrRes;	
		
	}else{			
		return $rs;	
	}	
}

/*****
#action: get_refund_rule_by_hours
#description: get refund policy data
#author: PKS
#created: 7 Aug 2015
*****/
function get_refund_rule_by_hours($hours){//if status is not passed, will returns all results
	
	$rs= db_select('refund_policy', 'rp')	
			->fields('rp')			
			->condition('rp.to_hours', $hours, '>')
			->condition('rp.status', 1, '=')
			->orderBy('rp.to_hours','ASC')
			->range(0,1)
			->execute()
			->fetch();
	
	return $rs;	
	
}


/*****
#action: get_refund_rule_by_deduction
#description: get refund policy data
#author: PKS
#created: 7 Aug 2015
*****/
function get_refund_rule_by_deduction($amount){
	
	$rs= db_select('refund_policy', 'rp')	
			->fields('rp')			
			->condition('rp.amount', $amount, '=')
			->condition('rp.status', 1, '=')
			->orderBy('rp.to_hours','ASC')
			->range(0,1)
			->execute()
			->fetch();
	
	return $rs;	
	
}



/*****
#action: unlock_curret_slots
#description: release the locked slots
#author: PKS
#created: 12 Aug 2015
*****/
function unlock_curret_slots(){
	
	//delete prev locked slots
	if(isset($_SESSION['cart_extra_info'])){
		
		$arrLockedSlots=$_SESSION['cart_extra_info']['slots'];
		unset($_SESSION['cart_extra_info']);
					
		if(!empty($arrLockedSlots)){				
			
			$drs = db_delete('slot_bookings')
			->condition('id',$arrLockedSlots, 'IN')
			->execute();
		}
	}	
}


/*****
#action: unlock_curret_slots
#description: release the locked slots
#author: PKS
#created: 12 Aug 2015
*****/
function unlock_slots_by_order_id_and_line_item_id($orderId,$lineItemId){
	
	//delete slots
	$urs=db_delete('slot_bookings')			
		->condition('line_item_id',$lineItemId, '=')
		->condition('order_id',$orderId, '=')
		->execute();		
	return $urs;
}

/*****
#action: get_product_display_link
#description: show product display link
#author: PKS
#created: 12 Aug 2015
*****/
function get_product_display_link($productId){
	
	$refNode= get_referencing_node_id($productId);
	$pLink = drupal_get_path_alias("node/". $refNode); 	
	return $pLink;					
	
}


/*****
#action: get_current_cart_item
#description: get current cart item
#author: PKS
#created: 12 Aug 2015
*****/
function get_current_cart_item($productId){
	
	global $user;
	$quantity=0;
	
	 // If the user actually has a cart order...
	if ($order = commerce_cart_order_load($user->uid)){
		
		// Count the number of product line items on the order.
		$wrapper = entity_metadata_wrapper('commerce_order', $order);
		$quantity = commerce_line_items_quantity($wrapper->commerce_line_items, commerce_product_line_item_types());	
		
	}
	
	return $quantity;
}
