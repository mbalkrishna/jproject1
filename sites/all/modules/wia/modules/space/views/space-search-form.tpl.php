<div class="container">
<div class="row">
<div class="col-sm-11 marginauto">

<div id="what" class="select-box col-sm-3">
<?php print render($form['space_type']);?>
</div> 

<div id="when" class="select-box col-sm-3">  
<?php print render($form['date_from']);?>
</div>

<div id="where" class="select-box col-sm-3">   
<?php print render($form['location']);?>
</div>

<?php
print render($form['flexible']); 
print render($form['donot_know']); 

print render($form['form_id']);
print render($form['form_build_id']);
print render($form['form_token']);
?>
<div class="select-box col-sm-3">   
<?php print render($form['submit']); ?>
</div>

</div>

</div>
</div>

<script type="text/javascript">
$(function(){
	
	var inputInst=null;
	

	
	//$("#what .ddTitleText .ddlabel").html('what');
	//$("#where .ddTitleText .ddlabel").html('where');
	
	$("#what .ddTitleText .ddlabel").removeClass('active');
	$("#where .ddTitleText .ddlabel").removeClass('active');
	
	
	$("#edit-space-type").change(function(){
		$("#what .ddTitleText .ddlabel").addClass('active');		
		
		if($.trim($(this).val())=='0' || $.trim($(this).val())==''){		
			$("#what .ddTitleText .ddlabel").removeClass('active');		
		}
		
	});
	
	$("#edit-location").change(function(){
		$("#where .ddTitleText .ddlabel").addClass('active');
		
		if($.trim($(this).val())=='0' || $.trim($(this).val())==''){						
			$("#where .ddTitleText .ddlabel").removeClass('active');
		}
	})
		
		
		$(".datepicker").datepicker({
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
	
});


function datepicker_addons(input){
	
	setTimeout(function () {
		var buttonPane = $(input).datepicker("widget");
			var html = '<div class="check-box-right"><div class="flexible checkbox checkbox-info"><input type="checkbox" class="styled" name="cb_flexible" id="flexible" onclick="manage_search_datepicker_cb(this);"><label for="flexible">flexible?</label></div>';
			html += '<div class="idonot checkbox checkbox-info"><input type="checkbox" id="idonot" class="styled" name="cb_donot_know" onclick="manage_search_datepicker_cb(this);"><label for="idonot">I don\'t know</label></div></div>';
		
		buttonPane.append(html);
		
		if($("input[name='flexible']").val()==1){
			
			$("input[name='cb_flexible']").attr("checked","checked");
								
		}else if($("input[name='donot_know']").val()==1){
			
			$("input[name='cb_donot_know']").attr("checked","checked");	
			
			//disable calender
			$("#ui-datepicker-div table").css({'pointer-events' : 'none', 'opacity' : 0.3});
			$("#ui-datepicker-div .ui-datepicker-header").css({'pointer-events' : 'none', 'opacity' : 0.3});
		
							
		}	
				
	}, 10);
}

</script>