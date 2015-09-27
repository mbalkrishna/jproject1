<div id='popup-content'>
<?php 
global $base_url;
$themePath=drupal_get_path('theme','wia');
if(!isset($currentTS))$currentTS=strtotime(date("Y-m-d"));
?>

<div id="calendar_wraper">
<div class="cal">
As of : &nbsp;<input class="form-text datepicker" type="text" name="current_date" value="<?php echo date("d/m/y",$currentTS);?>"/>
</div>
</div>

<div id="actions">
	<img id="loader_img" height="18" src="<?php echo $base_url;?>/sites/default/files/ajax-loader/loader-20.gif" style="display:none;"/>
    <div class="action-msg"></div>
</div>

<div id="slot_content">
<table>
<tr>
<th>Date/Slots</th>
<?php 
if($data['slots']){
	foreach($data['slots'] as $slot){?>
	<th><?php echo $slot->name;?></th>
<?php }
}?>
</tr>

<?php
$tmpCurTS=$currentTS;
while($tmpCurTS<strtotime("+7 days",$currentTS)){
	
	$day=date("Y-m-d",$tmpCurTS);
	?>

    <tr>
    <th><?php echo date("l jS M",$tmpCurTS);?></th>
    <?php
    if($data['slots']){
        foreach($data['slots'] as $slot){?>
            <td style="width:25%;">
            <?php
			$slotClass="slot-{$data['productId']}-{$slot->tid}-{$tmpCurTS}";			
			if($tmpCurTS<strtotime(date("Y-m-d"))){
				$slotClass.=" dead";
			}else{
				$slotClass.=" alive";
			}
            ?>
            
            <div class="<?php echo $slotClass;?> slot-wraper">
            <?php
			if(isset($data['bookings'][$day][$data['productId']][$slot->tid])){
				
				if($data['bookings'][$day][$data['productId']][$slot->tid]['status']=='Booked'){
					
					?><a class="booked"><div>&nbsp;</div></a><?php
					
				}elseif($data['bookings'][$day][$data['productId']][$slot->tid]['status']=='Processing'){
					
					?><a class="booked in-process"><div>&nbsp;</div></a><?php
					
				}elseif($data['bookings'][$day][$data['productId']][$slot->tid]['status']=="Unavailable"){
					
					?><a onclick="change_slot_status(<?php echo $data['productId'];?>,<?php echo $slot->tid;?>,'<?php echo $tmpCurTS;?>');" href="javascript:void(0);" class="unavailable" rel="available"><div>&nbsp;</div></a><?php
					
				}else{
					
					?><a onclick="change_slot_status(<?php echo $data['productId'];?>,<?php echo $slot->tid;?>,'<?php echo $tmpCurTS;?>');" href="javascript:void(0);" class="available" rel="unavailable"><div>&nbsp;</div></a><?php
					
				}
			
			}else{			
				?><a onclick="change_slot_status(<?php echo $data['productId'];?>,<?php echo $slot->tid;?>,'<?php echo $tmpCurTS;?>');" href="javascript:void(0);" class="available" rel="unavailable"><div>&nbsp;</div></a><?php				
			}?>
            </div></td>
            <?php		
        }
    }?>
    </tr>
	<?php 
	$tmpCurTS=strtotime("+1 day",$tmpCurTS);
}?>
</table>
</div><!--/slot_wraper-->

<div class="options-row">
<span class="available-help">&nbsp;</span>&nbsp;Available
<span class="booked-help">&nbsp;</span>&nbsp;Booked
<span class="unavailable-help">&nbsp;</span>&nbsp;Closed
</div>

<script type="text/javascript">
jQuery(function(){
	jQuery(".datepicker").datepicker({
		dateFormat: 'dd/mm/yy',		
		onSelect: function(curDate) {			
			get_slot_bookings(curDate);
		}
	});
	
	slot_init();	
	
});


function get_slot_bookings(curDate){	
	
	var curDate=curDate.replace(/\//g, '-');
	jQuery("#loader_img").show();
	jQuery.get( "<?php echo $base_url;?>/admin/get_slot_availability/<?php echo $data['productId'];?>/"+curDate, function( data ) {
		jQuery( "#slot_content").html(data);	
		jQuery("#loader_img").hide();
		slot_init();			
	});

}

function change_slot_status(productId,slotId,ts){
		
	if(confirm("Are you sure to change the slot status?")){
		
		jQuery("#loader_img").show();	
		var slotEle=jQuery(".slot-"+productId+"-"+slotId+"-"+ts);
		status=jQuery("a",slotEle).attr('rel');
	
	
		jQuery.post("<?php echo $base_url;?>/admin/change_slot_status", {productId: productId, slotId: slotId, ts: ts, status: status},function(data){
			jQuery("#loader_img").hide();
			jQuery('.action-msg').html(data).show().delay(2000).fadeOut('slow');
			
			if(jQuery('.action-msg p').hasClass('success')){
				if(jQuery.trim(status)=='unavailable'){			
					jQuery("a",slotEle).attr('rel','available');
					jQuery("a",slotEle).removeClass('available').addClass('unavailable');
					slot_init();	
				}else if(jQuery.trim(status)=='available'){
					
					jQuery("a",slotEle).attr('rel','unavailable');
					jQuery("a",slotEle).removeClass('unavailable').addClass('available');
					slot_init();	
				}
			}
			
		});
	}
}

function slot_init(){
	jQuery(".alive .unavailable").hover(function() {
        	jQuery("div",this).html('Click to Open/Available');
			jQuery(this).removeClass('unavailable').addClass('available');

    	},function(){
			jQuery("div",this).html('&nbsp;');
			jQuery(this).removeClass('available').addClass('unavailable');

		}
	);
	
	jQuery(".alive .available").hover(function() {
        	jQuery("div",this).html('Click to Close/Unavailable');
			jQuery(this).removeClass('available').addClass('unavailable');

    	},function(){
			jQuery("div",this).html('&nbsp;');
			jQuery(this).removeClass('unavailable').addClass('available');
			
		}	
	);
}


</script>
</div><!-- popup content wraper-->

