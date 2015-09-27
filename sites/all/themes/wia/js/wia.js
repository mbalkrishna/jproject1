/*********
Used: Head/Global JS
Author: PKS
Date: 14 July 15
*********/


$(document).ready(function(e) {
	try {
		$(".sel_dd").msDropDown();
		$(".sel_type").msDropDown();
	} catch(e) {}
	
	setHeight();  
	$(window).resize(function() {
		setHeight();
	});  
  
	$(document).ajaxComplete(function(vent, xhr, settings ) {	
		try {	
		
			$(".sel_dd").msDropDown();
			$(".sel_type").msDropDown();		
			$("#commerce-checkout-form-checkout select").msDropDown();
			
		} catch(e) {}
	});	
  
  
});

function setHeight() {
	windowHeight = $(window).innerHeight();
	$('.login-content').css('height', windowHeight);
	$(".login-content").css("height",(windowHeight - 100));
	
	$("iframe").css("min-height",(windowHeight - 100));	
	$("#map-canvas").css("min-height",(windowHeight - 100));
	
	$(".carousel-inner").css("height",(windowHeight - 170));
	
	$(".page-not-found").css("height",(windowHeight - 100));	
	
	
	

};


/*********
End: Head/Global JS
*********/



/*********
Used: Space detail/ book now page JS
Author: PKS
Date: 14 July 15
*********/
$(function(){
		
	$("#amenities-block input").click(function(){	
		render_amenity_prices();			
	});

});


function render_amenity_prices(){
	
	var amenities="";
	var totalAmenitiesPrice=0;
	var haveAmenities=false;	
	var curAmenities="<ul><li class='head'>amenities</li></ul>"
	
	var totalSlots=1;
	var noOfSlots=$(".hidden-time-slots").val().split(',');
	if(noOfSlots){
		totalSlots=noOfSlots.length;
	}		
	
	$("#amenities-block input[type=checkbox]").each(function(index, element) {
		
		if($(this).is(':checked')){
			
			haveAmenities=true;				
			amenities+=$(this).val()+',';				
			
			var aPrices=parseFloat($("#amenities-block .amenity_price_"+$(this).val()).val());
			
			if(isNaN(aPrices)){
				aPrices=0;
			}	
						
			aPrices=aPrices*totalSlots;			
			totalAmenitiesPrice+=aPrices;
			
			curAmenities+="<li><span class='title'>"+$("#amenities-block .amenity_title_"+$(this).val()).val()+"</span><span class='price'>£"+aPrices.toFixed(2)+"</span> </li>";
						
		}
	});		
	
	if(haveAmenities){
		$("#your-selections #current-amenities").html(curAmenities).show();		
	}else{
		$("#your-selections #current-amenities").html('').hide();
	}	
	
	amenities = amenities.replace(/\,$/,'');
	
	$(".hidden-amenities").val(amenities);		
	$(".hidden-amenity-total-price").val(totalAmenitiesPrice.toFixed(2));

	var basePrice=parseFloat($(".hidden-base-price").val());
	
	
	if(!isNaN(basePrice)){
		
		totalPrice=basePrice+totalAmenitiesPrice;
		$("#your-selections #total-booking-price span").html(totalPrice.toFixed(2));
		
	}	

}


//JS for slides and modal poups
$(function(){
			
	 $("#virtual-tour-btn").click(function(){
        $("#virtual-tour-modal").modal();
	});		
		
	$("#virtual-tour-slides").carousel();
	
	
	$("#floor-plan-btn").click(function(){
        $("#floor-plan-modal").modal();
	});		
		
	$("#floor-plan-slides").carousel();	
	
});


//JS for sticybar on space detail page
$(function() {
	var offset = $("#stickybar").offset();
	if(offset){
		var topPadding = 150;
		$(window).scroll(function() {
			if ($(window).scrollTop() > offset.top) {
				$("#stickybar").stop().animate({
					marginTop: $(window).scrollTop() - offset.top + topPadding
				});
			} else {
				$("#stickybar").stop().animate({
					marginTop: 0
				});
			};
		});
	}
});

function show_more_amenities(curObj){
	
	$('#amenities-block .hidden-amenities').toggle();
	if($(curObj).html().toLowerCase()=='more'){
		$(curObj).html('Less');
	}else{
		$(curObj).html('More');
	}
	
}


function load_slot_availability(productId,curDate){	
	
	//reset default actions messages on modal
	$("#slots-availability-modal .action-msg").html('');
	$("#slots-availability-modal .action-msg").removeClass('error');		
	
	
	$("#slots-availability-modal").modal('show');
	get_slot_availability(productId,curDate);
	 
	 $(".datepicker").datepicker({
		dateFormat: 'dd/mm/yy',	
		minDate: 0,	
		onSelect: function(curDate) {			
			get_slot_availability(productId, curDate);
		}
	});
	
	
}


function get_slot_availability(productId,curDate){		
	
	var arrDate=curDate.split('/');
	var date=arrDate[2]+"-"+arrDate[1]+"-"+arrDate[0];
	
	$("#loader_img").show();	
	$('#loader_img .loader-caption').html('&nbsp;please wait. slot availability is loading...');
	
	$.get(JS_BASE_URL+"/get_slot_availability/"+productId+"/"+date, function( data ) {
				
		$( "#slot_availability_content").html(data);	
		$("#loader_img").hide();	
		$('#loader_img .loader-caption').html('');
		slot_init();					
	});	

}

var curSlotSections=[];

function book_slot(productId,slotId,ts){	
		
	var selcEle=productId+"-"+slotId+"-"+ts;		
	var curEle=$(".slot-"+selcEle);
	$(curEle).toggleClass('cur-selection');	

	if($(curEle).hasClass('cur-selection')){
		curSlotSections.push(selcEle);
	}else{
		curSlotSections=remove_array_item(curSlotSections,selcEle);
	}
				
}

function remove_array_item(arr, item){
	
	arr = $.grep(arr, function(value) {
	  return value != item;
	});
	
	return arr;

}

function add_slots_to_cart(productId){
	
	var jsonData = JSON.stringify(curSlotSections);
	
	$("#loader_img").show();
	$('#loader_img .loader-caption').html('&nbsp;please wait. verifying your selection...');	
	
	$.post(JS_BASE_URL+"/lock_my_slots", { 'slots' : jsonData, 'product_id' : productId },function( data ) {
		
		var arrRes=JSON.parse(data);
		
		$("#loader_img").hide();
		$('#loader_img .loader-caption').html('');		
		
		if(arrRes.scope=='popup'){
			
			$("#slots-availability-modal .action-msg").html(arrRes.message);
			$("#slots-availability-modal .action-msg").addClass(arrRes.type);
			
		}else{
		
			$(".hidden-time-slots").val(arrRes.data.slots);					
			var totalSlotPrice=parseFloat(arrRes.data.slot_total_price);
			if(isNaN(totalSlotPrice)){
				totalSlotPrice=0;
			}
						
			$(".hidden-base-price").val(totalSlotPrice);					
			var curAmePrice=parseFloat($(".hidden-amenity-total-price").val());		
			if(isNaN(curAmePrice)){
				curAmePrice=0;
			}
			
			totalSlotPrice+=curAmePrice;
			
			$("#your-selections #total-booking-price span").html(totalSlotPrice.toFixed(2));
			
			$("#your-selections #current-slots").html(arrRes.message);			
			$("#slots-availability-modal .cancel").trigger('click');
			
			//recalculate amenities prices
			render_amenity_prices();
			
			//hide bottom book now button	
			
			if($(".book-now-bottom-row").is(":visible")){
				$(".book-now-bottom-row").hide();		
			}	
					
			//hide top book now button		
			/*if($("#book_now_top_btn").is(":visible")){
				$("#book_now_top_btn").hide();		
			}*/	
			
			$("#your-selections").show();
						
		}					
	
	});		
	
}


function slot_init(){
		
	$(".alive .available").hover(function() {
        	$("div",this).html('Book Now');
			$(this).removeClass('available').addClass('unavailable');

    	},function(){
			$("div",this).html('available');
			$(this).removeClass('unavailable').addClass('available');			
		}	
	);
	
	if(curSlotSections){
				
		var arrLen=curSlotSections.length;
		for (var i = 0; i < arrLen; i++) {
			$(".slot-"+curSlotSections[i]).addClass("cur-selection");
		}
	}
	
}

/*********
End: Space detail page JS
*********/


/*********
Start: Cart success page JS
*********/
function print_block(divId) {
     
	 var printContents = document.getElementById(divId).innerHTML;
     var originalContents = document.body.innerHTML;

     document.body.innerHTML = printContents;

     window.print();

     document.body.innerHTML = originalContents;
}



/*$(window).scroll(function(){
    if($(document).scrollTop() > 0) {
        $('#header').addClass('small-header');
		$('.logo-sec').addClass('logo-small');
		
		$('.logo-icon').addClass('logo-icon-small');
		$('.header-inner').addClass('header-inner-small');
		$('.top-nav').addClass('top-nav-small');
		$('.fixed-position').addClass('fixed-small');
	
		
    } else {
        $('#header').removeClass('small-header');
		$('.logo-sec').removeClass('logo-small');
		$('.logo-icon').removeClass('logo-icon-small');
		$('.header-inner').removeClass('header-inner-small');
		$('.top-nav').removeClass('top-nav-small');
		$('.fixed-position').removeClass('fixed-small');
    }
});*/


/*********
End: Cart success page JS
*********/


function proceed_file_upload(element,cur){
	
	$('#'+element).trigger('click');		
	$('#'+element).change(function(){
		$(cur).prev().html($(this).val());
		$('#'+element+'-button').show();		
	});	
}



/*********
Start: Search Asset page JS
*********/


function manage_search_datepicker_cb(curObj){	
	
	if($(curObj).is(":checked") && $(curObj).attr("name")=="cb_flexible"){		
		$("input[name='cb_donot_know']").removeAttr("checked");			

	}if($(curObj).is(":checked") && $(curObj).attr("name")=="cb_donot_know"){
		$("input[name='cb_flexible']").removeAttr("checked");
	}
	
	manage_datepicker_addons();
}


function manage_datepicker_addons(){

	if($("input[name='cb_donot_know']").is(":checked")){
		
		//disable calender
		$("#ui-datepicker-div table").css({'pointer-events' : 'none', 'opacity' : 0.3});
		$("#ui-datepicker-div .ui-datepicker-header").css({'pointer-events' : 'none', 'opacity' : 0.3});
		
		$(".datepicker").datepicker("hide");
		$(".datepicker").val("n/a");
	
		
	}else{
				
		if($(".datepicker").val()=="n/a"){
			$(".datepicker").val('');
		}					
		
		//enable calender		
		$("#ui-datepicker-div table").css({'pointer-events' : '','opacity' : ''});
		$("#ui-datepicker-div .ui-datepicker-header").css({'pointer-events' : '', 'opacity' : ''});	
	
	}
}


function datepicker_on_close(){
	
	if($("input[name='cb_flexible']").is(":checked")){
		
		$("input[name='flexible']").val(1);
	}else{
		$("input[name='flexible']").val(0);
	}
	
	if($("input[name='cb_donot_know']").is(":checked")){
		
		$("input[name='donot_know']").val(1);
	}else{
		$("input[name='donot_know']").val(0);
	}		
}


function manage_search_result_datepicker_cb(curObj){	
	
	if($(curObj).is(":checked") && $(curObj).attr("name")=="cb_flexible"){
				
		$("input[name='cb_donot_know']").removeAttr("checked");			
	}
	
	if($(curObj).is(":checked") && $(curObj).attr("name")=="cb_donot_know"){
		
		$("input[name='cb_flexible']").removeAttr("checked");		
	}
	
	manage_datepicker_addons_on_search_result();
}

function manage_datepicker_addons_on_search_result(){	
	
	if($("input[name='cb_flexible']").is(":checked")){
		
		$(".date_to").val('');
		$(".date_to").attr("disabled","disabled");
		
		if($(".date_from").val()=="n/a"){
			$(".date_from").val('');
		}			
		
		//enable calender		
		$("#ui-datepicker-div table").css({'pointer-events' : '','opacity' : ''});
		$("#ui-datepicker-div .ui-datepicker-header").css({'pointer-events' : '', 'opacity' : ''});	
		
		
	}else if($("input[name='cb_donot_know']").is(":checked")){
		
		//disable calender
		$("#ui-datepicker-div table").css({'pointer-events' : 'none', 'opacity' : 0.3});
		$("#ui-datepicker-div .ui-datepicker-header").css({'pointer-events' : 'none', 'opacity' : 0.3});
		
		$(".date_from").datepicker("hide");
		$(".date_from").val("n/a");
		
		$(".date_to").val('');
		$(".date_to").attr("disabled","disabled");		
		
	}else{
				
		if($(".date_from").val()=="n/a"){
			$(".date_from").val('');
		}	
		
		$(".date_to").removeAttr("disabled");			
		
		//enable calender		
		$("#ui-datepicker-div table").css({'pointer-events' : '','opacity' : ''});
		$("#ui-datepicker-div .ui-datepicker-header").css({'pointer-events' : '', 'opacity' : ''});		
	}
	
}


function show_more_filters(curObj){
	
	if($(".more-filters").is(':visible')){
		$(".more-filters").hide(500);	
		$(curObj).html('More Filters');
		$("input[name='more_filter']").val(0);
		
		$(".more-filters input[type='text']").val('');
		$(".more-filters input[type='checkbox']").attr('checked',false);
		
		
	}else{
		$(".more-filters").show(500);	
		$(curObj).html('less filters');
		$("input[name='more_filter']").val(1);
	}
}

/*********
End: Search Asset page JS
*********/