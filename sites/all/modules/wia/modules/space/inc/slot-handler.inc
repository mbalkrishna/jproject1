<?php
/*****
#action: space_admin_manage_slot_availability
#description: availability management by admin.
#author: PKS
#created: 05 June 2015
*****/
function space_admin_manage_slot_availability() {
	
	$productId=arg(2);
		
	//$name = 'time_slots';
	//$slots = taxonomy_vocabulary_machine_name_load($name);
	//$slots = taxonomy_get_tree($slots->vid);
	
	$slots=get_slots_by_product_id($productId);
	
	//get 7 days bookings from today
	$curDate=date("Y-m-d");
	$date7=date("Y-m-d",strtotime("+7 days",strtotime($curDate)));
	
	$rs=db_select('slot_bookings','sb')
	->fields('sb')
	->condition('product_id', $productId, '=')
	->condition('date',$date7, '<=')
	->condition('date',$curDate, '>=')
	->condition('status',array('Unavailable','Booked','Processing'), 'IN')
	->execute()
	->fetchAll();	
	
	
	$rs=format_bookings($rs);
	
	return theme('space_admin_manage_slot_availability',array('data'=>array('slots'=>$slots,'bookings'=>$rs,'productId'=>$productId)));
	
}


/*****
#action: space_admin_get_slot_availability
#description: get availability by date using ajax.
#author: PKS
#created: 05 June 2015
*****/
function space_admin_get_slot_availability() {
	
	$productId=arg(2);
	$date=arg(3);
	
	$ts=$date?strtotime($date):strtotime(date("Y-m-d"));
	
	//$name = 'time_slots';
	//$slots = taxonomy_vocabulary_machine_name_load($name);
	//$slots = taxonomy_get_tree($slots->vid);
	
	$slots=get_slots_by_product_id($productId);
	
	//get bookings for specified date
	$curDate=date('Y-m-d',$ts);
	$date7=date("Y-m-d",strtotime("+7 days",strtotime($curDate)));
	
	$rs=db_select('slot_bookings','sb')
	->fields('sb')
	->condition('product_id', $productId, '=')
	->condition('date',$date7, '<=')
	->condition('date',$curDate, '>=')
	->condition('status',array('Unavailable','Booked','Processing'), 'IN')
	->execute()
	->fetchAll();		
	
	$rs=format_bookings($rs);
		
	return array('current_ts'=>$ts,'slots'=>$slots,'productId'=>$productId,'bookings'=>$rs);
	
}

/*****
#action: space_admin_get_slot_availability_response_handler
#description: render availability.
#author: PKS
#created: 05 June 2015
*****/
function space_admin_get_slot_availability_response_handler($res){	
	
	$currentTS=$res['current_ts']?$res['current_ts']:strtotime(date("Y-m-d"));
	$out="<table><tr><th>Date/Slots</th>";
   
    if($res['slots']){
        foreach($res['slots'] as $slot){
        	$out.="<th>".$slot->name."</th>";
    	}
    }
    $out.="</tr>";
      
    $tmpCurTS=$currentTS;
    while($tmpCurTS<strtotime("+7 days",$currentTS)){
    	
		$day=date("Y-m-d",$tmpCurTS);
		
		$out.="<tr>";
		$out.="<th>".date("l jS M",$tmpCurTS)."</th>";
		
		if($res['slots']){
			foreach($res['slots'] as $slot){
				
				$out.="<td style='width:25%;'>";
				
				$slotClass="slot-{$res['productId']}-{$slot->tid}-{$tmpCurTS}";			
				if($tmpCurTS<strtotime(date("Y-m-d"))){
					$slotClass.=" dead";
				}else{
					$slotClass.=" alive";
				}				
				
				$out.="<div class=\"{$slotClass} slot-wraper\">";
				
				if(isset($res['bookings'][$day][$res['productId']][$slot->tid])){
					
					if($res['bookings'][$day][$res['productId']][$slot->tid]['status']=='Booked'){
						
						 $out.="<a class='booked'><div>&nbsp;</div></a>";
						
					}elseif($res['bookings'][$day][$res['productId']][$slot->tid]['status']=='Processing'){
						
						 $out.="<a class='booked in-process'><div>&nbsp;</div></a>";
						
					}elseif($res['bookings'][$day][$res['productId']][$slot->tid]['status']=="Unavailable"){
						
						 $out.="<a onclick=\"change_slot_status({$res['productId']},{$slot->tid},'{$tmpCurTS}');\" href='javascript:void(0);' class='unavailable' rel='available'><div>&nbsp;</div></a>";
						
					}else{
						
						 $out.="<a onclick=\"change_slot_status({$res['productId']},{$slot->tid},'{$tmpCurTS}');\" href='javascript:void(0);' class='available' rel='unavailable'><div>&nbsp;</div></a>";
						
					}
				
				}else{			
					 $out.="<a onclick=\"change_slot_status({$res['productId']},{$slot->tid},'{$tmpCurTS}');\" href='javascript:void(0);' class='available' rel='unavailable'><div>&nbsp;</div></a>";				
				}
				$out.="</div></td>";
			
			}
		}
		$out.="</tr>";			
        $tmpCurTS=strtotime("+1 day",$tmpCurTS);
    }   
		
    $out.="</table>";
	
	echo $out;
	
}


/*****
#action: space_admin_change_slot_status
#description: change slot status.
#author: PKS
#created: 05 June 2015
*****/
function space_admin_change_slot_status() {
	
	if($_POST){
		
		global $user;
		$date=date('Y-m-d',$_POST['ts']);
		$status=ucfirst($_POST['status']);
		if(!in_array($status,array('Unavailable','Available')))return array('message'=>"Invalid status passed.",'type'=>'error');		
		
		if($_POST['ts']<strtotime(date("Y-m-d")))return array('message'=>"You can not change the status of past dates.",'type'=>'error');
		
		
		$rs = db_select('slot_bookings', 'sb')
		->fields('sb')
		->condition('product_id', $_POST['productId'], '=')
		->condition('slot_id', $_POST['slotId'], '=')
		->condition('date',$date, '=')
		->execute()
		->fetch();		
		
		
		if($rs){
		
			$ars=db_update('slot_bookings')->fields(array(
			'status' => $status,
			'notes' => $status,
			'uid' => $user->uid))
			->condition('product_id', $_POST['productId'], '=')
			->condition('slot_id', $_POST['slotId'], '=')
			->condition('date',$date, '=')
			->execute();
			  
		}else{
		
			$ars=db_insert('slot_bookings')->fields(array(
			'product_id' =>$_POST['productId'],
			'slot_id' =>$_POST['slotId'],
			'date' => $date,
			'status' => $status,
			'notes' => $status,
			'uid' => $user->uid))
		  	->execute();
		
		}
		
		return array('message'=>"Slot status updated successfully",'type'=>'success');
	}
	
	return array('message'=>"Slot status could not be updated. Please try again",'type'=>'error');
	
}

/*****
#action: space_admin_change_slot_status_update
#description: render update message of change status.
#author: PKS
#created: 05 June 2015
*****/
function space_admin_change_slot_status_response_handler($res){
	
	if($res['type']=='success'){
		echo "<p class='success'>{$res['message']}</p>";
	}else{
		echo "<p class='error'>{$res['message']}</p>";	
	}
	
}


/*****
#action: space_get_slot_availability
#description: get availability by date using ajax - front user.
#author: PKS
#created: 05 June 2015
*****/
function space_get_slot_availability() {
	
	$productId=arg(1);
	$date=arg(2);

	$ts=$date?strtotime($date):strtotime(date("Y-m-d"));
	
	//$name = 'time_slots';
	//$slots = taxonomy_vocabulary_machine_name_load($name);
	//$slots = taxonomy_get_tree($slots->vid);
	
	$slots=get_slots_by_product_id($productId);	
	
	//get bookings for specified date
	$curDate=date('Y-m-d',$ts);
	$date7=date("Y-m-d",strtotime("+7 days",strtotime($curDate)));
	
	$rs=db_select('slot_bookings','sb')
	->fields('sb')
	->condition('product_id', $productId, '=')
	->condition('date',$date7, '<=')
	->condition('date',$curDate, '>=')
	->condition('status',array('Unavailable','Booked','Processing'), 'IN')
	->execute()
	->fetchAll();		
		
	$rs=format_bookings($rs);

	return array('current_ts'=>$ts,'slots'=>$slots,'productId'=>$productId,'bookings'=>$rs);
	
}

/*****
#action: space_render_slot_availability
#description: render availability - front user.
#author: PKS
#created: 05 June 2015
*****/
function space_get_slot_availability_response_handler($res){	
	
	$currentTS=$res['current_ts']?$res['current_ts']:strtotime(date("Y-m-d"));
	
	$out="<table class='real-time-availbility lowercase'><tr class='light-border'><td class='grey-font'>&nbsp;</td>";
   
    if($res['slots']){
        foreach($res['slots'] as $slot){
        	$out.="<td class='grey-font'>".$slot->name."</td> ";
    	}
    }
    $out.="</tr>";
      
    $tmpCurTS=$currentTS;
    while($tmpCurTS<strtotime("+7 days",$currentTS)){
    	
		$day=date("Y-m-d",$tmpCurTS);
		
		$out.="<tr class='light-border'>";
		$out.="<td class='month'>".date("l jS M",$tmpCurTS)."</td>";
		
		if($res['slots']){
			foreach($res['slots'] as $slot){
				
				//get slot master details
				$slotDB=get_slot_details_by_id($slot->tid);					
				$slotStTime=$slotDB->field_start_time['und'][0]['value'];				
				$dateTS2=strtotime($day.' '.$slotStTime);				
				
				$out.="<td class='slot'>";
				
				$slotClass="slot-{$res['productId']}-{$slot->tid}-{$tmpCurTS}";			
				
				$dateExpired=false;
				
				if($dateTS2<strtotime(date("Y-m-d H:i"))){//validate past date bookings
									
					$dateExpired=true;
					$slotClass.=" dead";
					
				}else{					
					$slotClass.=" alive";
				}				
				
				$out.="<div class=\"{$slotClass} slot-wraper\">";
				
				if(!$dateExpired){
				
					if(isset($res['bookings'][$day][$res['productId']][$slot->tid])){
						
						if($res['bookings'][$day][$res['productId']][$slot->tid]['status']=='Booked'){
							
							 $out.="<a class='booked'><div>booked</div></a>";
							
						}elseif($res['bookings'][$day][$res['productId']][$slot->tid]['status']=="Unavailable"){
							
							 $out.="<a class='unavailable'><div>unavailable</div></a>";
							
						}elseif($res['bookings'][$day][$res['productId']][$slot->tid]['status']=="Processing"){// for processing slots exclude my selection
							$mySelection=array();
							if(isset($_SESSION['cart_extra_info']))$mySelection=$_SESSION['cart_extra_info']['slots'];	
							
							if(in_array($res['bookings'][$day][$res['productId']][$slot->tid]['data']->id,$mySelection)){
								$out.="<a onclick=\"book_slot({$res['productId']},{$slot->tid},'{$tmpCurTS}');\" href='javascript:void(0);' class='available' rel='unavailable'><div>available</div></a>";
							}else{
								 $out.="<a class='unavailable in-process'><div>unavailable</div></a>";
							}
							
							
						}else{
							
							 $out.="<a onclick=\"book_slot({$res['productId']},{$slot->tid},'{$tmpCurTS}');\" href='javascript:void(0);' class='available' rel='unavailable'><div>available</div></a>";
							
						}
					
					}else{			
						 $out.="<a onclick=\"book_slot({$res['productId']},{$slot->tid},'{$tmpCurTS}');\" href='javascript:void(0);' class='available' rel='unavailable'><div>available</div></a>";				
					}
				}else{
					 $out.="<a class='unavailable'><div>unavailable</div></a>";
				}
				
				$out.="</div></td>";
			
			}
		}
		$out.="</tr>";					
        $tmpCurTS=strtotime("+1 day",$tmpCurTS);
    }  
	$out.="</table>";  
	
	echo $out;
}