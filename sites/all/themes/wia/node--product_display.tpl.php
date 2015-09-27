<?php
$_nid=$node->nid;
$_productId=$node->field_product_reference['und'][0]['product_id'];
$pwrapper = entity_metadata_wrapper('commerce_product', $_productId);
global $base_url,$theme_path;

$bookingCalStartDate=date(DATE_FORMAT);
if(isset($_SESSION['form_state'])){
	
	if($_SESSION['form_state']['values']['date_from'])
	$bookingCalStartDate=$_SESSION['form_state']['values']['date_from']; 
};

if($pwrapper){?>

<div id="carousel-example-generic" class="carousel slide" data-ride="carousel">
<div class="carousel-inner" role="listbox">
<?php	
	$slides=$pwrapper->field_photos->value();
	if($slides){	
		$slideCount=0;		
		foreach($slides as $fImg){
			$img = array(
				'style_name' => 'product_gallery',
				'path' => $fImg['uri'],
				'alt' => $fImg['alt'],
				'title' => $fImg['title']
			);
			if(!$slideCount){$slideCount++;
				echo "<div class='item active'>";
			}else{
				echo "<div class='item'>";
			}
			
			print theme('image_style',$img);
			echo "</div>";
		}
		?>
		
	  <!-- Controls -->
      <a class="left carousel-control" href="#carousel-example-generic" role="button" data-slide="prev">
        <span class="glyphicon glyphicon-chevron-left" aria-hidden="true"></span>
        <span class="sr-only">Previous</span>
      </a>
      <a class="right carousel-control" href="#carousel-example-generic" role="button" data-slide="next">
        <span class="glyphicon glyphicon-chevron-right" aria-hidden="true"></span>
        <span class="sr-only">Next</span>
      </a>		
	  <?php
	}else{?>
		<img src="<?php echo $base_url."/".$theme_path;?>/images/slider-placeholder.jpg" width="100%"/><?php
	}
	?>
</div>
</div>
 
<!--booking now section-->
<div class="container">
<div class="row">
<div class="col-lg-3 col-md-4 col-sm-12 float-right">
<div class="sticky-scroll-box">
<div id="booking_price" class="book-now-sticky price-section">		
<p>starting from</p>
<?php $basePrice=$content['product:commerce_price']['#items'][0]['original']['amount'];?>
<p class="price"><?php echo CURRENCY_SYMBOL;?><span class="total-price"><?php print commerce_currency_format($basePrice);?></span></p>
<p id="book_now_top_btn" class="book_now_btn">
<a href="javascript:void(0);" class="book-now-btn" onclick="load_slot_availability(<?php echo $_productId;?>,'<?php echo $bookingCalStartDate;?>');">book now</a>
</p>
<script>
$(function() {//initialize the base price
	$(".hidden-base-price").val(<?php echo $basePrice;?>);
   
});
</script>
</div>
<div class="help-link">
<a href="<?php echo $base_url;?>/help-faqs" target="_blank">click here for help</a>
</div>
</div>

<div id="your-selections" style="display:none;" >
<div class="mini-cart">
<h2>your booking selection</h2>
<a href="javascript:void(0);" class="edit" onclick="load_slot_availability(<?php echo $_productId;?>,'<?php echo $bookingCalStartDate;?>');" title="edit booking"><i class="fa fa-pencil"></i></a>
<div id="current-slots">
</div>

<div id="current-amenities" style="display:none;">
</div>

<div id="total-booking-price">
total : <div><?php echo CURRENCY_SYMBOL;?><span></span></div> 
</div>

</div>

<a href="javascript:void(0);" id="asset_detail_page_checkout" class="checkout btn" onclick="javascript:$('#add-to-cart-form .form-submit').trigger('click');">add to cart</a>

</div>

</div>

<!-- time slots row-->



<!-- /time slots row-->
<div id="add-to-cart-form" style="display:none">
<?php print render($content['field_product_reference']); ?>
</div>
<!--/booking now section-->

<?php
if(!empty($_SESSION['global']['messages'])){
	print $_SESSION['global']['messages'];?>
    <script type="application/javascript">
	$(document).ready(function(){
		$("html, body").animate({ 
			scrollTop: $('.messages').offset().top-400 
		}, 1000);
	});
	</script>
    
    <?php
}?>


<div class="col-lg-9 col-md-8 col-xs-12">
<h1 class="asset-title"><?php print render($content['product:title']);?></h1>
<p class="sub-heading"><?php print render($content['product:field_full_address']);?></p>

<?php 
$filledStars=0;
if(isset($content['product:field_space_rating']['#items'][0]['value'])){
	$filledStars=$content['product:field_space_rating']['#items'][0]['value'];
}
?>
<p class='space-rating'>
<?php 
$rateLimit=5;
if($filledStars>$rateLimit)$filledStars=$rateLimit;

$blankStars=$rateLimit-$filledStars;

for($i=0; $i<$filledStars; $i++){?>
	<span><img src="<?php echo $base_url."/".$theme_path;?>/images/review-active-icon.png" /></span>
<?php }

for($i=0; $i<$blankStars; $i++){?>
	<span><img src="<?php echo $base_url."/".$theme_path;?>/images/review-icon.png" /></span>
<?php }?>

</p>

<h2>about this listing</h2>
<p class="sub-heading"><?php print render($content['product:field_description']);?></p>

<div class="devider margintop20"></div>

<!-- price row-->
<div class="row">
<div class="col-sm-4">
<label>Prices</label>
</div>
<div class="col-sm-8">
<?php

$slotPrices=get_slot_prices($_productId);		
$slots=get_slots_by_product_id($_productId);

foreach ($slots as $term) {		

	$slotPrice=commerce_currency_format($slotPrices[$term->tid]['price']);	
	$curSym=CURRENCY_SYMBOL;	
	?>    
	<ul class='lowercase'>
	<li class="slots">		
	<?php if(!empty($slotPrice)) {
		echo "{$curSym}{$slotPrice}&nbsp;";
	}
	 echo $term->name;?>
    </li>
    </ul>		
    <?php	
}
?>    
</div>
<div class="col-sm-4">
</div>
</div>
<!-- /price row-->

<div class="devider margintop20 paddingbottom20"></div>

<!-- floor plan and virtual tour row-->
<div class="row">
<div class="col-sm-4">
<label>View</label>
</div>
<div class="col-sm-8">
<p>
<?php
$vslides=$pwrapper->field_virtual_tour->value();
if($vslides){?>
	<a href="javascript:void(0);" id="virtual-tour-btn" class="btn left marginright30">virtual tour</a>
<?php }else{?>
	<a href="javascript:void(0);" class="btn left marginright30 disabled">virtual tour</a>
    <?php
}
$flslides=$pwrapper->field_floor_plan->value();	
if($flslides){?>
	<a href="javascript:void(0);" id="floor-plan-btn" class="btn left">floor plans</a>
<?php }else{?>
	<a href="javascript:void(0);" class="btn left disabled">floor plans</a>
<?php
}
?>
</p> 

<!--virtual tour image slides-->

<!-- Modal -->
 <?php            
if($vslides){?>
<div class="modal fade" id="virtual-tour-modal" role="dialog">
<div class="modal-dialog ">

	<div class="modal-content">
        <div class="modal-header">
          <button type="button" class="close" data-dismiss="modal">&times;</button>
          <h4 class="modal-title">Virtual Tour</h4>
        </div>
 	
   		<div id="virtual-tour-slides" class="carousel slide" data-ride="carousel">
        <div class="carousel-inner" role="listbox">
       		<?php
                $slideCount=0;		
                foreach($vslides as $vsImg){
                    $img = array(
                        'style_name' => 'virtual_tour',
                        'path' => $vsImg['uri'],
                        'alt' => $vsImg['alt'],
                        'title' => $vsImg['title']
                       );
                    if(!$slideCount){$slideCount++;
                        echo "<div class='item active'>";
                    }else{
                        echo "<div class='item'>";
                    }
                    
                    print theme('image_style',$img);
                    echo "</div>";
                }
                ?>
                
              <!-- Controls -->
              <a class="left carousel-control" href="#virtual-tour-slides" role="button" data-slide="prev">
                <span class="glyphicon glyphicon-chevron-left" aria-hidden="true"></span>
                <span class="sr-only">Previous</span>
              </a>
              <a class="right carousel-control" href="#virtual-tour-slides" role="button" data-slide="next">
                <span class="glyphicon glyphicon-chevron-right" aria-hidden="true"></span>
                <span class="sr-only">Next</span>
              </a>	                
        </div>
        </div>
        
   </div><!--/modal content-->
    
</div><!--/modal dialogue-->
</div>
<?php }?>
<!--modal-->
<!--/virtual tour image slides-->


<!--floor plans image slides-->
<!-- Modal -->
<?php
 if($flslides){
?>
<div class="modal fade" id="floor-plan-modal" role="dialog">
<div class="modal-dialog">

	<div class="modal-content">
        <div class="modal-header">
          <button type="button" class="close" data-dismiss="modal">&times;</button>
          <h4 class="modal-title">Floor Plan</h4>
        </div>
 	
   		<div id="floor-plan-slides" class="carousel slide" data-ride="carousel">
        <div class="carousel-inner" role="listbox">
        <?php	           
           	
                $slideCount=0;		
                foreach($flslides as $flsImg){
                    $img = array(
                        'style_name' => 'floor_plan',
                        'path' => $flsImg['uri'],
                        'alt' => $flsImg['alt'],
                        'title' => $flsImg['title']
                       );
                    if(!$slideCount){$slideCount++;
                        echo "<div class='item active'>";
                    }else{
                        echo "<div class='item'>";
                    }
                    
                    print theme('image_style',$img);
                    echo "</div>";
                }
                ?>
                
              <!-- Controls -->
              <a class="left carousel-control" href="#floor-plan-slides" role="button" data-slide="prev">
                <span class="glyphicon glyphicon-chevron-left" aria-hidden="true"></span>
                <span class="sr-only">Previous</span>
              </a>
              <a class="right carousel-control" href="#floor-plan-slides" role="button" data-slide="next">
                <span class="glyphicon glyphicon-chevron-right" aria-hidden="true"></span>
                <span class="sr-only">Next</span>
              </a>                  
        </div>
        </div>
        
   </div><!--/modal content-->
    
</div><!--/modal dialogue-->
</div>
<!--modal-->
<!--/floor plans image slides-->
<?php
}
?> 
</div>
</div>
<!--/floor plan and virtual tour slides-->

<div class="devider margintop10 paddingbottom20"></div>

<!-- /book now bottom section-->


<div class="row book-now-bottom-row">
<div class="col-sm-4">
<label>Confirm Date</label>
</div>
<div class="book_now_btn col-sm-4">
<a href="javascript:void(0);" class="book-now-btn book-now-btn2" onclick="load_slot_availability(<?php echo $_productId;?>,'<?php echo $bookingCalStartDate;?>');">confirm now <span></span> </a>
</div>
<div class="col-sm-4"></div>
</div>

<div class="devider margintop20 paddingbottom20 book-now-bottom-row"></div>
<!-- /book now bootom section-->



<!--amenities row-->
<div class="row">
<div class="col-md-4 col-sm-12">
<label>Amenities</label>
</div>

<div id="amenities-block" class="col-md-8 col-sm-12"> 
<div class="row">
<?php
   
    $amenityPrices=get_amenity_prices($_productId);		
	$amtree=get_amenities_by_product_id($_productId);
	$rowCount=1;	
	$dataIndex=0;
	$classAme='';
	$hideRow='';
	$rowCount=1;
	
	if($amtree){
		foreach ($amtree as $term) {		
			
			$amenityPrice=commerce_currency_format($amenityPrices[$term->tid]['price']);	
			$curSym=CURRENCY_SYMBOL;				
			
			?>   
          <div class="col-md-6">
			<div class="checkbox checkbox-info">
			<input type="checkbox" class="styled" name="amenity[]" value="<?php echo $term->tid;?>" id="ame-<?php echo $term->tid;?>"/>		
			<label for="ame-<?php echo $term->tid;?>">
			<?php echo $term->name;?>
			<?php if(!empty($amenityPrice)) {
				echo "({$curSym}{$amenityPrice})";
			}else{
				echo "(Free)";
			}
			?>
			</label>
            <input type="hidden" name="amenity_price[<?php echo $term->tid;?>]" class="amenity_price_<?php echo $term->tid;?>" value="<?php echo $amenityPrice;?>"/>
            
            <input type="hidden" name="amenity_title[<?php echo $term->tid;?>]" class="amenity_title_<?php echo $term->tid;?>" value="<?php echo $term->name;?>"/>
            
			</div>	
            </div>
                       	
			<?php	
			$dataIndex++;
																
			if($dataIndex%2==0){	
				
				$rowCount++;
				
				if($dataIndex>2){
					$hideRow='display:none;';
					$classAme='hidden-amenities';
				}	
				
				
										
				echo "</div><div class='row {$classAme}' style='{$hideRow}'>";
			}	
			
		}
	}else{
		echo "<div class='no-amenities'>No Amenities available</div>";
	}
	?>    
</div>
</div>

<?php if(isset($hideRow) && $rowCount>3){?>

<div class="col-md-4"></div>
<div class="col-md-8 col-sm-12">
<p><a href="javascript:void(0)" onclick="show_more_amenities(this);" class="btn left">More</a></p>
</div>

<?php }?>

</div>
<!--/amenities row-->

<div class="devider margintop20 paddingbottom20"></div>

<!-- caterer row-->
<div class="row">
<div class="col-sm-4">
<label>Catering</label>
</div>
<div class="col-sm-8 lowercase">
<?php 
$caterers=NULL;

if(isset($content['product:field_caterer']['#items']))
$caterers=$content['product:field_caterer']['#items'];

if($caterers){?>
	<ul class='lowercase'><?php
	foreach ($caterers as $_caterer) {	
		?>    
    	<li>
		<?php if(isset($_caterer['taxonomy_term']->field_url_link['und'][0]['value']) &&  $_caterer['taxonomy_term']->field_url_link['und'][0]['value']!="" ) { ?>
        <a href="<?php echo $_caterer['taxonomy_term']->field_url_link['und'][0]['value'];?>" target="_blank"><?php echo $_caterer['taxonomy_term']->name;?></a>
		<?php }  else {		?>
		 <?php echo $_caterer['taxonomy_term']->name;?>
		
		<?php } ?>
		
		</li>
       <?php
	}?>
    </ul>
    <?php
}else{
	echo "Caterer(s) not available.";
}
?>

</div>
</div>
<!-- /caterer row-->
<div class="devider margintop20 paddingbottom20"></div>

<?php if(isset($_SESSION['search_result']) && !empty($_SESSION['search_result']) && count($_SESSION['search_result']['data'])>1){?>

<!-- more venues and similar venues-->
<h2 class="paddingbottom10">more venues at this listing</h2>
<div class="result-box asset-venue-box">
<div class="row">
<?php 
$resultData=$_SESSION['search_result'];
$resultData=$resultData['data'];
shuffle($resultData);
$indxCount=1;

foreach($resultData as $_product){
	
	$__productId=$_product->product_id;
	
	if($__productId!==$_productId){
	
		if($indxCount>3)break;
		$indxCount++;
		
		
		
		$pwrapper = entity_metadata_wrapper('commerce_product', $__productId);			
		
		$spt = $pwrapper->field_space_type->value();
		$price = $pwrapper->commerce_price->value();
				
		/*$spaceType='';
		if($spt){
			foreach($spt as $_spt){
				
				if(isset($_spt->name))
				$spaceType.=$_spt->name.', ';
			}
		}
		$spaceType=trim($spaceType,", ");
		*/
		
		$roomName=$pwrapper->field_room_name->value();
		$fullAdd=$pwrapper->field_full_address->value();	
		$spaceTitle=$pwrapper->title->value();;	
		$loc = $pwrapper->field_location->value();	
		
		$refNode= get_referencing_node_id($__productId);
		$pLink = url('node/'. $refNode); 	
			
		$fImg = $pwrapper->field_featured_image->value();
		
		?>
		
		<div class="col-sm-4">
		<a href="<?php echo $pLink ;?>" class="img-box">
		
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
		
		<span class="price-tag">
		<p class="small-font">starting from</p>
		<p class="big-font"><?php echo CURRENCY_SYMBOL;?><?php echo commerce_currency_format($price['amount']);?></p>
		</span>
		</a>
		<h2><a href="<?php echo $pLink ;?>"><?php echo $spaceTitle;?></a></h2>
		<p><?php echo $roomName;?></p>
		<p><?php echo $fullAdd;?></p>
		</div>
		<?php
	}
}
	
?>
</div><!--/row-->
</div> <!--/asset-venue-box-->
<!--/ more venues and similar venues-->
<?php }?>

</div>
</div>

</div>
</div>
</div>
<!-- /more venues and similar venues-->


<!-- availability row modal-->

<!-- Modal -->
<div class="modal fade" id="slots-availability-modal" role="dialog">
<div class="modal-dialog lg">

	<div class="modal-content">
        <div class="modal-header">
           <h2 class="modal-title">real-time availability</h2>
        </div>
        
        <div id="calendar_wraper">
        <div class="cal">
       		<span class="as-of"> as of :</span>&nbsp;<span class="as-of-cal"><input class="form-text datepicker input calendar-icon" type="text" name="current_date" value="<?php echo $bookingCalStartDate;?>" readonly="readonly"/></span>
        </div>
        </div>
        
        <div id="actions">
            <div id="loader_img" class="loader"  style="display:none;">
            	<img class="loader-img" height="30" src="<?php echo $base_url;?>/sites/default/files/ajax-loader/loader-20.gif"/>
                <span class="loader-caption"></span>
            </div>
            <div class="action-msg"></div>
        </div>
        
        <div id="slot_availability_content">        
        </div>
        
        <div class="options-row">
        <div class="row">
        <div class="col-md-8">
        <span class="available-help"></span><span class="color-text">available</span>
        <span class="cur-selection-help"></span><span class="color-text">your selection</span>
        <span class="booked-help"></span><span class="color-text">booked</span>
        <span class="unavailable-help"></span><span class="color-text">closed</span>
        </div>
        
        <div class="col-md-4 text-right">
         <a href="javascript:void(0);" class="btn1 left cancel" data-dismiss="modal">cancel</a> &nbsp;&nbsp;
         <a href="javascript:void(0);" class="btn left" onclick="add_slots_to_cart(<?php echo $_productId;?>);">Apply</a>
        </div>
        </div>
        </div>        
    </div>    
 </div>
 </div>
<!--/ Modal -->
<div id="slot_cur_selections" style="display:none;">
</div>
<!-- /availability row-->

   
<?php }else{?>
	<div class='space-not-found'>space not found.</div>
    
<?php
}?>

<script>
/*$(document).ready(function () {  
  var top = $('.sticky-scroll-box').offset().top;
  $(window).scroll(function (event) {
		var y = $(this).scrollTop();
		if (y >= top) {
		  $('.sticky-scroll-box').addClass('');
		} else {
		  $('.sticky-scroll-box').removeClass('');
		}
		
  	});
});*/
</script>
<style>
.add_to_cart_msg{display:none;}
.mini-cart{padding:13px;}
</style>