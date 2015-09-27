<?php 
$themePath=drupal_get_path('theme','space');
global $base_url; 
?>
<div class="col-sm-5 no-padding fixed-position" >
<div class="google-maps">
<iframe src="<?php echo $base_url."/gmap"?>" width="600" frameborder="0" style="border: 0px none; min-height: 369px;"></iframe>
</div>
</div>

<div class="col-sm-7 paddingleftfixed no-padding">

<div class="search-result-form">


<div class="row">
<div class="col-sm-3">
<label><?php echo $form['location']['#title'];?></label>
</div>
<div class="col-sm-9">
<?php
unset($form['location']['#title']);
print render($form['location']);?>
<div class="devider"></div>
</div>
</div>


<div class="row">
<div class="col-sm-3">
<label><?php echo $form['date_from']['#title'];?></label>
</div>
<div class="col-sm-9">
<div class="row">
<div class="col-lg-5 col-md-6 col-sm-6 col-xs-6 ">
<?php
unset($form['date_from']['#title']);
print render($form['date_from']);?>
</div>
<div class="col-lg-5 col-md-6 col-sm-6 col-xs-6">
<?php print render($form['date_to']);?>
</div>
</div>
<div class="devider"></div>
</div>
</div>


<div class="row">
<div class="col-sm-3">
<label><?php echo $form['space_type']['#title'];?></label>
</div>
<div class="col-sm-9">
<div class="row">
<div class="col-lg-5 col-md-6 col-sm-8 col-xs-9">
<?php 
unset($form['space_type']['#title']);
print render($form['space_type']);?>
</div>    
</div>
<div class="devider"></div>
</div>
</div>


<div class="row">
<div class="col-sm-3">
<label>Price</label>
</div>
<div class="col-sm-9 price-slider">
<div class="amount1 price"><?php echo CURRENCY_SYMBOL;?><span><?php echo $form['min_price']['#value'];?></span></div>
<div class="amount price"><?php echo CURRENCY_SYMBOL;?><span><?php echo $form['max_price']['#value'];?></span></div>
<div id="slider-range"></div>

<?php print render($form['min_price']);?>
<?php print render($form['max_price']);?>
<div class="devider"></div>
</div>
</div>


<div class="row">
<div class="col-sm-3">
<label><?php print $form['time_slot']['#title'];?></label>
</div>
<div class="col-sm-9 check-box-right">
<?php 
unset($form['time_slot']['#title']);
print render($form['time_slot']);?>
<div class="devider"></div>
</div>
</div>

<?php 

$moreFStyle="display:none;";
$moreFContent="More Filters";

if($form['more_filter']['#value']){
	$moreFStyle="display:block;";
	$moreFContent="Less Filters";
}

?>

<div class="more-filters" style="<?php echo $moreFStyle;?>">

<div class="row">
<div class="col-sm-3">
<label><?php echo $form['capacity']['#title'];?></label>
</div>
<div class="col-sm-9">
<?php
unset($form['capacity']['#title']);
print render($form['capacity']);?>
<div class="devider"></div>
</div>
</div>


<div class="row">
<div class="col-sm-3">
<label><?php print $form['amenities']['#title'];?></label>
</div>
<div class="col-sm-9 check-box-right">
<?php 
unset($form['amenities']['#title']);
print render($form['amenities']);?>
<div class="devider"></div>
</div>
</div>

</div><!--/more-filters-->

<?php 
//hidden fields
print render($form['min_price']);
print render($form['max_price']);

print render($form['flexible']); 
print render($form['donot_know']);
print render($form['more_filter']);

print render($form['form_id']);
print render($form['form_build_id']);
print render($form['form_token']);
?>

<div class="row">
<div class="col-sm-3">&nbsp;</div>
<div class="col-sm-9">
<a href="javascript:void(0);" onclick="show_more_filters(this);" class='filter-button btn normal'><?php echo $moreFContent;?></a>
<div class="devider"></div>
<?php print render($form['submit']); ?>
<div class="devider"></div>
</div>
</div>


<div class="row">
<div class="col-lg-6 col-md-5 col-sm-3 col-xs-3 text-right label-text">
<?php print $form['price_order']['#title'];?>:
</div>
<div class="col-lg-6 col-md-7 col-sm-9 col-xs-9">
<?php 
unset($form['price_order']['#title']);
print render($form['price_order']);?>
</div>
</div>
   
</div><!--/search form-->

<div class="result-box">

<?php
if(isset($form['search_result']) && !empty($form['search_result'])){

	print render($form['search_result']);
	print render($form['pager']);
	
}?>
</div>
</div>

<script type="text/javascript">
$(function(){
	
	var inputInst=null;
	
	$(".date_from").datepicker({
		dateFormat: '<?php echo JS_DATE_FORMAT;?>',	
		minDate: 0,
		beforeShow: function (input, inst) {  
			inputInst=input;      
			datepicker_addons(inputInst);				

   		},		
		onChangeMonthYear: function (year, month, inst)
		{	
			datepicker_addons(inputInst);
			datepicker_on_close();
        },
		 		
		onClose: function (selectedDate) {
			
			datepicker_on_close();	
   		}
	});	
	
	
	$(".date_to").datepicker({
		dateFormat: '<?php echo JS_DATE_FORMAT;?>',		
		minDate: 0,	
	});	
	
	
	<?php $basePriceRange=get_price_range();?>
			
	$( "#slider-range" ).slider({
		range: true,
		min: <?php echo floor($basePriceRange['min_price']);?>,
		max: <?php echo ceil($basePriceRange['max_price']);?>,
		values: [<?php echo $form['min_price']['#value'];?>, <?php echo $form['max_price']['#value'];?>],
		slide: function( event, ui ) {
			$( ".min-price" ).val(ui.values[ 0 ]);
			$( ".max-price" ).val(ui.values[ 1 ]);
			
			$( ".amount1 span" ).html(ui.values[ 0 ]);
			$( ".amount span" ).html(ui.values[ 1 ]);
		}
	});
		
	$("#edit-price-order").change(function(){
		$("#space-search-result-form").submit();
	});	
	
});

function datepicker_addons(input){
	
	setTimeout(function () {
		var buttonPane = $(input).datepicker("widget");
			var html = '<div class="check-box-right"><div class="flexible checkbox checkbox-info"><input type="checkbox" class="styled" name="cb_flexible" id="flexible" onclick="manage_search_result_datepicker_cb(this);"><label for="flexible">flexible?</label></div>';
			html += '<div class="idonot checkbox checkbox-info"><input type="checkbox" id="idonot" class="styled" name="cb_donot_know" onclick="manage_search_result_datepicker_cb(this);"><label for="idonot">I don\'t know</label></div></div>';
		
		buttonPane.append(html);
		
		if($("input[name='flexible']").val()==1){
			
			$("input[name='cb_flexible']").attr("checked","checked");
								
		}else if($("input[name='donot_know']").val()==1){	
				
			$("input[name='cb_donot_know']").attr("checked","checked");	
						
			$("#ui-datepicker-div table").css({'pointer-events' : 'none', 'opacity' : 0.3});	
			$("#ui-datepicker-div .ui-datepicker-header").css({'pointer-events' : 'none', 'opacity' : 0.3});
							
		}	
				
	}, 10);
}
</script>