<?php 
global $base_url,$theme_path;

if(!empty($data['data'])){
	
	$productIds=array();		
	foreach($data['data'] as $_product){
		
		$productIds[]=$productId=$_product->product_id;
		
		$pwrapper = entity_metadata_wrapper('commerce_product', $productId);			
		
		$spt = $pwrapper->field_space_type->value();
		$price = $pwrapper->commerce_price->value();
				
		$spaceType='';
		if($spt){
			foreach($spt as $_spt){
				
				if(isset($_spt->name))
				$spaceType.=$_spt->name.', ';
			}
		}
		
		$spaceType=trim($spaceType,", ");
		
		$roomName=$pwrapper->field_room_name->value();
		$fullAdd=$pwrapper->field_full_address->value();	
		$spaceTitle=$pwrapper->title->value();;	
		$loc = $pwrapper->field_location->value();	
	
		$refNode= get_referencing_node_id($productId);
		$pLink = drupal_get_path_alias("node/". $refNode); 					
		
		
		$fImg = $pwrapper->field_featured_image->value();
		
		?>
		<div class="row">   
			<div class="col-lg-7 col-md-7 col-sm-7">
			<a href="<?php echo $pLink;?>">
			<?php 
			if($fImg && $fImg['uri']){
				
				$img = array(
				'style_name' => 'featured_thumbnail',
				'path' => $fImg['uri'],
				'alt' => $fImg['alt'],
				'title' => $fImg['title']			
				);
			   
				print theme('image_style',$img);
			 }else{?>
				<img src="<?php echo $base_url."/".$theme_path;?>/images/no-image.jpg" />
			 <?php }
			?>
			</a>
			
			 <span class="price-tag">
			  <p class="small-font">starting from</p>
			  <p><?php echo CURRENCY_SYMBOL;?><?php echo commerce_currency_format($price['amount']);?></p>
			 </span>
		 
		   </div>
			
			<div class="col-lg-4 col-md-5 col-sm-5">
            				
				<h2><a href="<?php echo $pLink;?>"><?php echo $spaceTitle;?></a></h2>
				<p class='space-types'><?php echo $roomName;?></p>
				<!--
				<p class='space-types'><?php echo $spaceType;?></p>
				-->
				<p class='post-code'><?php echo $fullAdd;?></p>
				<!--
                <p class='location'><?php echo $loc->name;?></p>
                -->
			</div>  			
		</div>
	
	 <?php }
	 
	$gmapLatLong=get_latlong_by_product_ids($productIds);		
	$gMapCenter=get_gmap_center($gmapLatLong);
	$_SESSION['gmap_data']=array('locations'=>$gmapLatLong,'center'=>$gMapCenter);//assign lat and long for google graph		

}else{?>
	<div class="row">
	<div class='no-result'>No Result Found.</div>
    </div>
<?php }?>