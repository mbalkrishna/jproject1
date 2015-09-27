<?php
/*****
#action: space_search_form
#description: space search home form.
#author: PKS
#created: 10 June 2015
*****/

function space_search_form($form, &$form_state) {
	
	
	$locations=get_locations('list');
			
	$form['location'] = array(
		'#title' => FALSE,
		'#type' => 'select',
		'#options' => array(''=>'where')+$locations,
		'#theme_wrappers'=>array()
	);
	$form['location']['#attributes']['class'][] = 'sel_dd';
	
	
	$form['date_from'] = array(
		'#title' => FALSE,
		'#type' => 'textfield',
		'#size' => 12,
		'#attributes' =>array('placeholder' => t('when')),
		'#theme_wrappers'=>array()
	);
	
	$form['date_from']['#attributes']['class'][] = 'datepicker';
	$form['date_from']['#attributes']['class'][] = 'date-input-css';
	$form['date_from']['#attributes']['readonly'] = 'readonly';
	
	$form['flexible'] = array(
		'#type' => 'hidden',
		'#default_value' =>0
	);
	
	$form['donot_know'] = array(
		'#type' => 'hidden',
		'#default_value' =>0
	);
	
	
	$spaceTypes=get_space_types('list');
	
	$form['space_type'] = array(
		'#title' => FALSE,
		'#type' => 'select',
		'#options' => array(''=>'what')+$spaceTypes,
		'#theme_wrappers'=>array()
	);
	$form['space_type']['#attributes']['class'][] = 'sel_dd';	

	$form['referer'] = array(
		'#type' => 'hidden',
		'#default_value' =>'home-search'
	);
	
	$form['submit'] = array(
		'#value' => 'Search',
		'#type' => 'submit'			
	);		
	
	$form['submit']['#attributes']['class'][]='search btn';		
	
	return $form;
	
 } 

/*****
#action: space_search_form_validate
#description: validate space search form.
#author: PKS
#created: 10 June 2015
*****/
function space_search_form_validate($form, &$form_state) {
	
	if(empty($form_state['values']['location'])){
		 form_set_error('location', 'the field where can not be empty.');
	}
	
	if(empty($form_state['values']['space_type'])){
		 form_set_error('space_type', 'the field what can not be empty.');
	}
	
	//validate date for flexible status
	if($form_state['values']['flexible'] && empty($form_state['values']['date_from'])){
		 form_set_error('date_from', 'the field when can not be empty with flexible mode.');
	}
		
}


/*****
#action: space_search_form_submit
#description: on submit get result and render.
#author: PKS
#created: 10 June 2015
*****/
function space_search_form_submit($form, &$form_state) {
    	
	$form_state['redirect']='space-search-result';	
	$_SESSION['form_state']=$form_state;		
} 


/*****
#action: space_search_result_form
#description: space search form.
#author: PKS
#created: 10 June 2015
*****/

function space_search_result_form($form, &$form_state) {
	

	$todayTS=strtotime(date('Y-m-d'));	
			
	$form_state=$_SESSION['form_state'];
	
	$_location=$form_state['values']['location'];
	$_fromDate=$form_state['values']['date_from'];
	$_toDate=$form_state['values']['date_to'];
	$_spaceType=$form_state['values']['space_type'];	
	$_flexible=$form_state['values']['flexible'];
	$_donotKnow=$form_state['values']['donot_know'];
	
	$locations=get_locations('list');
			
	$form['location'] = array(
		'#title' => t('Place'),
		'#type' => 'select',
		'#default_value'=>$_location,
		'#options' => $locations,
		'#theme_wrappers'=>array()
	);
	$form['location']['#attributes']['class'][] = 'sel_dd';
	
	$form['date_from'] = array(
		'#title' => t('Date'),
		'#type' => 'textfield',		
		'#size' => 12,
		'#default_value'=>$_fromDate,
		'#theme_wrappers'=>array()
	);
	
	$form['date_from']['#attributes']['class'][] = 'date_from date-input-css input calendar-icon';
	$form['date_from']['#attributes']['placeholder'][] = 'start';
	$form['date_from']['#attributes']['readonly'] = 'readonly';
	
	
	$form['date_to'] = array(
		'#title' => FALSE,
		'#type' => 'textfield',
		'#size' => 12,
		'#default_value'=>$_toDate,
		'#theme_wrappers'=>array()
		
	);
	$form['date_to']['#attributes']['class'][] = 'date-input-css input calendar-icon date_to';
	$form['date_to']['#attributes']['placeholder'][] = 'finish';
	$form['date_to']['#attributes']['readonly'] = 'readonly';
	
	if($_flexible==1 || $_donotKnow==1){
		
		$form['date_to']['#attributes']['disabled'] = 'disabled';
	}
	
	
	$form['flexible'] = array(
		'#type' => 'hidden',
		'#default_value' =>$_flexible
	);
	
	$form['donot_know'] = array(
		'#type' => 'hidden',
		'#default_value' =>$_donotKnow
	);
	
	
	$spaceTypes=get_space_types('list');
	
	$form['space_type'] = array(
		'#title' => t('Type'),
		'#type' => 'select',
		'#default_value'=>$_spaceType,
		'#options' => $spaceTypes,
		'#theme_wrappers'=>array()
	);
	
	$form['space_type']['#attributes']['class'][] = 'sel_dd';
	
	$priceRange=get_price_range();
	
	$form['min_price'] = array(
		'#type' => 'hidden',
		'#default_value' =>floor($priceRange['min_price'])
	);	
	$form['min_price']['#attributes']['class'][] = 'min-price';
	
	
	$form['max_price'] = array(
		'#type' => 'hidden',
		'#default_value' =>ceil($priceRange['max_price'])
	);	
	$form['max_price']['#attributes']['class'][] = 'max-price';	
	
	
	
	$timeSlots=get_time_slots('list');
	
	$form['time_slot'] = array(
		'#title' => t('Time Slot'),
		'#type' => 'checkboxes',
		'#options' => $timeSlots,
		'#prefix' => '<div class="checkbox checkbox-info">',
		'#suffix' => '</div>',
		'#theme_wrappers'=>array()		
	);
	$form['time_slot']['#attributes']['class'][]='styled';

	
	$form['capacity'] = array(
		'#title' => t('Capacity'),
		'#type' => 'textfield',
		'#size' => 12,
		'#theme_wrappers'=>array()
	);
	$form['capacity']['#attributes']['placeholder'][]='number of people';
	
	$form['capacity']['#attributes']['class'][]='input half';
	
	
	$form['multispace'] = array(
		'#title' => t('Multispace'),
		'#description' => t('ieg. more than one room'),
		'#type' => 'checkbox',
		'#theme_wrappers'=>array()
	);
	
	$amenities=get_amenities('list');
	
	$form['amenities'] = array(
		'#title' => t('Amenities'),
		'#type' => 'checkboxes',
		'#options' => $amenities,
		'#prefix' => '<div class="checkbox checkbox-info amenitites-w">',
		'#suffix' => '</div>',
		'#theme_wrappers'=>array()
	);
	
	$form['amenities']['#attributes']['class'][]='styled';
	
	
	$form['submit'] = array(
		'#value' => 'Show Listings',
		'#type' => 'submit'
			
	);		
	
	$form['more_filter'] = array(
		'#type' => 'hidden',
		'#default_value' =>0
	);	
			
	
	$form['price_order'] = array(
		'#title' => 'Display',
		'#type' => 'select',
		'#options' => array('asc'=>'price low to high','desc'=>'price high to low'),
		'#theme_wrappers'=>array()
	);	
	$form['price_order']['#attributes']['class'][] = 'sel_dd';
	
	$form['submit']['#attributes']['class'][]='btn';	
	
	
	/*--search result--*/
	if(empty($form_state['data'])){//get result
		$rs=get_space_result($form_state['values']);		
		$form_state['data']=$rs;
	}
	
	if(!empty($form_state['data'])){
		
		$searchResult=$form_state['data'];			
		$_SESSION['search_result']=$searchResult;					
		$rs=theme('space_search_result',array('data'=>$searchResult));			
		$form['search_result'] = array('#markup' => $rs);			
		$form['pager'] = array('#markup' => $form_state['data']['pager']);
	}	
	/*--/search result--*/
	
	
	return $form;
	
 } 

/*****
#action: space_search_result_form_validate
#description: validate space search form.
#author: PKS
#created: 10 June 2015
*****/
function space_search_result_form_validate($form, &$form_state) {
	
	if(empty($form_state['values']['location'])){
		 form_set_error('location', 'the field location can not be empty.');
	}
	
	if(empty($form_state['values']['space_type'])){
		 form_set_error('space_type', 'the field space type can not be empty.');
	}
	
	//validate date for flexible status
	if($form_state['values']['flexible'] && empty($form_state['values']['date_from'])){
		 form_set_error('date_from', 'the field start date can not be empty with flexible mode.');
	}

	if(!empty($form_state['values']['date_from']) && !empty($form_state['values']['date_to'])){
		
		$stDate=convet_date_tosys_format($form_state['values']['date_from']);
		$endDate=convet_date_tosys_format($form_state['values']['date_to']);
	
		if(strtotime($endDate)<strtotime($stDate)){
			form_set_error('date_from', 'finish date can not be before date of start date.');
		}
	}
	
}


/*****
#action: space_search_result_form_submit
#description: on submit get result and render.
#author: PKS
#created: 10 June 2015
*****/

function space_search_result_form_submit($form, &$form_state) {    
	
	$rs=get_space_result($form_state['values']);	
	$form_state['data']=$rs;	
	$_SESSION['form_state']=$form_state;//save submitted data to session for pagination
	
	$form_state['redirect']='space-search-result';	
	$form_state['rebuild'] = TRUE;	
} 

/*****
#action: get_space_result
#description: find result.
#author: PKS
#created: 10 June 2015
*****/
function get_space_result($arrFilter=null){	
		
	$totalTimeSlots=get_time_slots('count');			
	
	$selectedSlots=null;
	if(isset($arrFilter['time_slot'])){
		$selectedSlots=array_values($arrFilter['time_slot']);	
		$remove = array(0);
		$selectedSlots = array_diff($selectedSlots, $remove);
	
		if(count($selectedSlots)>0){
			 $totalTimeSlots=count($selectedSlots);
		}
	}

	$today=date('Y-m-d');;
	$todayTS=strtotime($today);
	
	$stDate=null;
	if(isset($arrFilter['date_from']) && !empty($arrFilter['date_from'])){
		$stDate=convet_date_tosys_format(trim($arrFilter['date_from']));
		$stDateTS=strtotime($stDate);
		
	}else{		
	
		$stDateTS=$todayTS;		
	}			
	
	
	$endDate=null;
	if(isset($arrFilter['date_to']) && !empty($endDate)){
		
		$endDate=convet_date_tosys_format(trim($arrFilter['date_to']));
		$endDateTS=strtotime($endDate);	
		
	}else{// assign the same start date if end date not given

		$endDateTS=strtotime("+7 days",$stDateTS);
	}	
		
	//override date as per flexible and i donot know status.	
	if($arrFilter['flexible'] && $arrFilter['flexible']==1){	
	
		$tmpStDateTS=$stDateTS;
		$stDateTS=strtotime("-3 days",$stDateTS);		
		$endDateTS=strtotime("+3 days",$tmpStDateTS);
		
		if($stDateTS<=$todayTS){
			
			$dateDiff=floor($todayTS-$stDateTS)/(24*60*60);
			$endDateTS=strtotime("+{$dateDiff} days",$endDateTS);			
			$stDateTS=$todayTS;
		}				
		
	}elseif($arrFilter['donot_know'] && $arrFilter['donot_know']==1){
		
		$stDateTS=$todayTS;		
		$endDateTS=strtotime("+7 days",$stDateTS);		
	}
			
	$stDate=date('Y-m-d',$stDateTS);
	$endDate=date('Y-m-d',$endDateTS);
	
	$dateDiff=1;
	if($endDateTS>$stDateTS){
		$dateDiff=ceil(abs($endDateTS-$stDateTS)/(60*60*24));
	}
	
	$totalPosibleSlots= $totalTimeSlots * $dateDiff;	
				
	$query = db_select('commerce_product', 'p')->extend('PagerDefault')->limit(5);;
	$query->fields('p', array('product_id','title','sku'));	
	
	//join with locations
	$query->leftJoin('field_data_commerce_price', 'pr', "p.product_id = pr.entity_id");
	$query->fields('pr', array('commerce_price_amount'));
	
	//join with locations
	$query->leftJoin('field_data_field_location', 'l', "p.product_id = l.entity_id");
	$query->fields('l', array('field_location_tid'));
	
	//join with space types
	$query->leftJoin('field_data_field_space_type', 'st', "p.product_id = st.entity_id");
	$query->fields('st', array('field_space_type_tid'));
	
	//join with amenities
	$query->leftJoin('amenity_prices', 'am', "p.product_id = am.product_id AND am.amenity_status=1");
	$query->fields('am', array('amenity_id','amenity_status'));
	
	//join with capacity
	$query->leftJoin('field_data_field_capacity', 'cp', "p.product_id = cp.entity_id");
	$query->fields('cp', array('field_capacity_value'));	
	
	//join with slots
	$query->leftJoin('slot_prices', 'sp', "p.product_id = sp.product_id");
	$query->fields('sp', array('slot_id','amount','slot_status'));
	
	//join with bookings
	$query->leftJoin('slot_bookings', 'sb', "p.product_id = sb.product_id");
	$query->fields('sb', array('status'));	
	
	$where="1";
		
	$subQuery="SELECT count(sb1.slot_id) FROM wia_slot_bookings sb1 WHERE sb1.status IN('Unavailable','Booked') AND sb1.product_id=p.product_id AND (sb1.date>={$stDate} AND sb1.date<={$endDate})";	

	
	if(!empty($arrFilter['location'])){//condition on location
		$where.=" AND l.field_location_tid={$arrFilter['location']}";
	}
	
	if(!empty($arrFilter['space_type'])){//condition on space type
		$where.=" AND st.field_space_type_tid ={$arrFilter['space_type']}";
	}
		
	if(!empty($arrFilter['capacity'])){//condition on capacity filter
		$where.=" AND cp.field_capacity_value >={$arrFilter['capacity']}";
	}
	
	if(!empty($arrFilter['amenities'])){//condition on amenities filter
		
		$amenities=array_values($arrFilter['amenities']);	
		$remove = array(0);
		$amenities = array_diff($amenities, $remove);	
		
		$totalAme=count($amenities);
		
		if($amenities){
			$strAmenities=implode(",",$amenities);		
			$where.=" AND {$totalAme}=(SELECT COUNT(am2.amenity_id) FROM wia_amenity_prices as am2 WHERE am2.amenity_id IN ({$strAmenities}) AND am2.amenity_status=1 AND am2.product_id=p.product_id)";
		}
	}	
	
	if(!empty($selectedSlots)){//condition on slots filter
		
		$strSelectedSlots=implode(',', $selectedSlots);
		$where.=" AND sp.slot_id IN ({$strSelectedSlots})";	
				
		$subQuery.="  AND sb1.slot_id IN ({$strSelectedSlots})";
			
	}	
	
	if(isset($arrFilter['min_price']))
	$minPrice=$arrFilter['min_price'];
	
	if(empty($minPrice))$minPrice=0;
	
	if(isset($arrFilter['max_price']))
	$maxPrice=$arrFilter['max_price'];
	
	if(empty($maxPrice))$maxPrice=99999999999;	
	
		
	$where.=" AND (sp.amount >={$minPrice} AND sp.amount<={$maxPrice})";
	$where.=" AND ({$totalPosibleSlots}>({$subQuery})) AND p.status=1 AND sp.slot_status=1";
	
	$query->where($where);
	
	$pOrder='asc';
	
	if(isset($arrFilter['price_order']))
	$pOrder=$arrFilter['price_order'];
	
	$query->orderBy("pr.commerce_price_amount", $pOrder);
	$query->groupBy("p.product_id");
	  $query->limit(9);
	$result = $query->execute()->fetchAll();	
	 $count_res = count($result);
	//echo $query->__toString();die;
	$pager= theme('pager',array('quantity'=>9));
	
		
	
	return array('data'=>$result,'pager'=>$pager);
	
}



/*****
#action: space_show_gmap
#description:
#author: PKS
#created: 05 June 2015
*****/
function space_show_gmap() {
	
	$mapData=arg(1);
	return $mapData;
	
}

/*****
#action: space_show_gmap_response_handler
#description:
#author: PKS
#created: 05 June 2015
*****/
function space_show_gmap_response_handler($mapData=NULL,$centerLat=51.54087705,$centerLong=-0.10098745){		
	
	global $base_url, $theme_path;
	
	if(empty($mapData)){
		
		$index=0;
		$gMapData=null;
		
		if(isset($_SESSION['gmap_data'])){
			$gMapData=$_SESSION['gmap_data'];
			unset($_SESSION['gmap_data']);
		}
		
		$mapData = "[";		
		if($gMapData){
			$index=0;
			
			foreach($gMapData['locations'] as $mapLoc){$index++;
				$mapTitle=addslashes($mapLoc['title']);
				$mapData.="['{$mapTitle}', {$mapLoc['latitude']}, {$mapLoc['longitude']},{$index}],";	
			}
			
			$mapData=trim($mapData,",");
		}
		$mapData.="]";
		
		if($gMapData['center']['latitude'])
		$centerLat=$gMapData['center']['latitude'];
		
		if($gMapData['center']['longitude'])
		$centerLong=$gMapData['center']['longitude'];
		
	}
	
	?>
	<script src="https://maps.googleapis.com/maps/api/js?v=3.exp&signed_in=true"></script>
    <style>
    body{ margin:0;}
    </style>
	<script type="text/javascript">
    
    function gmap_init(){
           
        var locations = <?php echo $mapData;?>;
        
        var mapStyles =   [
        {
            stylers: [
                /*{ hue: '#8800ff' },
                { saturation: -100 }*/
            ]
            },{
            featureType: 'road',
            elementType: 'geometry',
            stylers: [
                { lightness: 100 },
                { visibility: 'simplified' }
            ]
            },{
            featureType: 'road',
            elementType: 'labels',
            stylers: [
                { visibility: 'off' }
            ]
        }];
    
    
        var map = new google.maps.Map(document.getElementById('map-canvas'), {
          zoom: 12,
          center: new google.maps.LatLng(<?php echo $centerLat;?>, <?php echo $centerLong;?>),
          styles: mapStyles,
          mapTypeId: google.maps.MapTypeId.ROADMAP
		 
        });
    
        var infowindow = new google.maps.InfoWindow();
    
        var marker, i;
    
        for (i = 0; i < locations.length; i++) {  
          marker = new google.maps.Marker({
            position: new google.maps.LatLng(locations[i][1], locations[i][2]),
            map: map,
			icon: "<?php echo $base_url."/".$theme_path;?>/images/gmap_pin.png"
          });
    
          google.maps.event.addListener(marker, 'click', (function(marker, i) {
            return function() {
              infowindow.setContent(locations[i][0]);
              infowindow.open(map, marker);
            }
          })(marker, i));
        }
    }
    
    google.maps.event.addDomListener(window, 'load', gmap_init);
    </script>
    <div id="map-canvas" style="height:100%;width:100%"></div>   
	<?php
}