<?php 
$themePath=drupal_get_path('theme','wia');
global $user;

if(in_array('standard user',$user->roles)){//theme used for front user
?>
    <div class="row">
    <div class="col-sm-8 marginauto">
    <div class="search-result-form list-space">
    
    <h1>list your space</h1>
    <?php if(isset($messages)){print $messages;} ?>
    <div class="devider margintop40 paddingbottom20"></div>    
  	<div class="row">
    <div class="col-sm-12">
    <p><label><?php echo $form['field_space_type']['und']['#title'];unset($form['field_space_type']['und']['#title']);?></label><span>*</span></p>
    
   	<?php print render($form['field_space_type']); ?>
    </div>
    </div>
    
    <div class="devider margintop20 paddingbottom20"></div>
   
    <div class="row">
      <div class="col-sm-12" id="where">
     <p><label><?php echo $form['field_location']['und']['#title'];unset($form['field_location']['und']['#title']);?></label><span>*</span></p>
     <?php print render($form['field_location']);?>
     </div>
    </div>
    
     <div class="devider margintop20 paddingbottom20"></div>
    
    <div class="row">
    <div class="col-sm-12">
    <p><label>Title and Description</label></p>   
    <?php 
	unset($form['title']['#title']);
	print render($form['title']); 
	
	unset($form['field_room_name']['und'][0]['value']['#title']);	
	print render($form['field_room_name']); 	
  
	unset($form['field_description']['und'][0]['value']['#title']);	
	print render($form['field_description']); ?>
    </div>
    </div>
    
    <div class="devider margintop20 paddingbottom20"></div>
    
    <div class="field-wraper row">
    <div class="col-sm-12">
    <p><label>Amenities</label></p>
    <?php print render($form['amenities']);?>
    </div>
    </div>
    
    <?php 
    //hide default descriptions with file field widget
    $form['field_featured_image']['und'][0]['#description']='';
    $form['field_photos']['und']['#file_upload_description']='';
    $form['field_virtual_tour']['und']['#file_upload_description']='';
    $form['field_floor_plan']['und']['#file_upload_description']='';
    ?>
    
     <div class="devider margintop20 paddingbottom20"></div>
    
    <div class="row">
     <div class="col-sm-12">
    <p><label>Featured Image</label></p>
    <?php 	
	unset($form['field_featured_image']['und'][0]['#title']);
	print render($form['field_featured_image']);?>
    </div>
    </div>
    
     <div class="devider margintop20 paddingbottom20"></div>
    
    <div class="row">
    <div class="col-sm-12">
     <p><label>Photos (banner images)</label></p>
    <?php
	 unset($form['field_photos']['und']['#title']);
	 print render($form['field_photos']); ?>
     </div>
    </div>
    
     <div class="devider margintop20 paddingbottom20"></div>
    
    <div class="row">
    <div class="col-sm-12">
     <p><label>Virtual Tour Images</label></p>
    <?php
	 unset($form['field_virtual_tour']['und']['#title']);
	 print render($form['field_virtual_tour']); ?>
     </div>
    </div>
    
     <div class="devider margintop20 paddingbottom20"></div>
    
    <div class="row">
    <div class="col-sm-12">
     <p><label>Floor Plan Images</label></p>
    <?php	
	unset($form['field_floor_plan']['und']['#title']); 
	print render($form['field_floor_plan']); ?>
    </div>
    </div>
    
     <div class="devider margintop20 paddingbottom20"></div>
    
    <div class="field-wraper row">
    <div class="col-sm-12">
    <p><label>Price</label><span class="form-required">*</span></p>
    <?php print render($form['time_slots']);?>
    </div>
    </div>
          
     <div class="devider margintop20 paddingbottom20"></div>
    
    <div class="row">
    <div class="col-sm-12">
     <p><label>Capacity and Address</label><span>*</span></p>
   	 <?php
	  unset($form['field_capacity']['und'][0]['value']['#title']);
	  print render($form['field_capacity']); ?>   
    <?php 
	 unset($form['field_full_address']['und'][0]['value']['#title']);
	 print render($form['field_full_address']);
	 
	 unset($form['field_postcode']['und'][0]['value']['#title']);
	 print render($form['field_postcode']); 
	  ?>
     </div>
    </div> 
     
    <div class="auto-display" style="display:none;">
    <?php
    //for auto display module
    print render($form['auto_display']);
    print render($form['commerce_apd_referenced_nid']);
    ?>
    </div>
    <?php
    print render($form['form_id']);
    print render($form['form_build_id']);
    print render($form['form_token']);
	?>
    
    <div class="frm-action" style="display:none;">
    <?php print render($form['actions']);?>
    </div>
    
   	<div class="devider margintop40 paddingbottom30"></div>
  
 	<input name="add_asset" type="button" value="submit" class="btn left"  onclick="javascript:$('#edit-submit').trigger('click');"/>
  
    <div class="paddingbottom50"></div>    
    
    </div>
    </div>
    </div> 
    
    <style>
		#edit-field-description .filter-wrapper{display:none;}
		.form-type-managed-file label{display:none;}
		#edit-actions a{opacity:0;}		
	</style>   
    <?php
    

}else{//theme used for admin
?>


	<?php print render($form['field_space_type']); ?>
    <?php print render($form['field_location']);?>
    
    <?php print render($form['title']); ?>
    <?php print render($form['field_room_name']); ?>    
    <?php print render($form['field_description']); ?>
    
    <div class="field-wraper">
    <label>Amenities</label>
    <?php print render($form['amenities']);?>
    </div>
    
    <?php 
    //hide default descriptions with file field widget
    $form['field_featured_image']['und'][0]['#description']='';
    $form['field_photos']['und']['#file_upload_description']='';
    $form['field_virtual_tour']['und']['#file_upload_description']='';
    $form['field_floor_plan']['und']['#file_upload_description']='';
    ?>
    
    <?php print render($form['field_featured_image']);?>
    <?php print render($form['field_photos']); ?>
    <?php print render($form['field_virtual_tour']); ?>
    <?php print render($form['field_floor_plan']); ?>
    
    <div class="field-wraper">
    <label>Prices <span title="This field is required." class="form-required">*</span></label>
    <?php print render($form['time_slots']);?>
    </div>
    
    <?php print render($form['field_caterer']); ?>
    
    
    <?php print render($form['field_capacity']); ?>
    <?php print render($form['field_full_address']); ?>
    <?php print render($form['field_postcode']); ?>
    <?php print render($form['field_space_rating']); ?>
    <?php print render($form['status']); ?>
    
    <div class="auto-display" style="display:none;">
    <?php
    //for auto display module
    print render($form['auto_display']);
    print render($form['commerce_apd_referenced_nid']);
    ?>
    </div>
    <?php
    print render($form['form_id']);
    print render($form['form_build_id']);
    print render($form['form_token']);	
	print render($form['actions']);

}?>
<style>
.image-widget-data .image-field input.form-file{display:none;}
.image-widget-data .image-field input[value='Upload']{display:none;}
</style>
<script>
function limits(obj, limit){

   var words = $(obj).val().match(/\S+/g).length;

    if (words > 250) {
      // Split the string on first 200 words and rejoin on spaces
      var trimmed = $(obj).val().split(/\s+/, 250).join(" ");
      // Add a space at the end to make sure more typing creates new words
      $(obj).val(trimmed + " ");
    }
    
 }


 $("#edit-field-description-und-0-value").on('keydown', function(e) {

    limits($(this), 250);
});


$("#commerce-product-ui-product-form").submit(function(e){
  
   var valid = false;   
   $(".small-input").each(function() {
     if($(this).val() !="" ){
    var numbers = /^[0-9]+$/; 
	if(Math.floor(parseFloat($(this).val())) >=1 ) {
		valid = true;
		}	
		else { alert("Please Enter Valid Price.");   e.preventDefault();}
		
     if($(this).val().match(numbers)){
		valid = true;
		}	  
		
else { alert("Please enter valid price.");   e.preventDefault();}

}

   });
   
});

$(".small-input").keypress(function(event) {
  if ( event.which == 45 || event.which == 189 ) {
      event.preventDefault();
   }
     
   
   
});

</script>