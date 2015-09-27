<!--Footer start-->
<div class="footer">
<div class="container">

<div class="row">

<div class="col-sm-9">
<?php
print theme('links', array('links' => menu_navigation_links('menu-footer-menu'),'attributes' => array('class' => false)));
?>
</div>
<div class="col-sm-3 copy-right">
<?php
$copyright=variable_get('copyright');
if(!empty($copyright)){
	echo $copyright;
}else{?>
© where its at <?php echo date('Y');?>. All Rights Reserved
<?php }?>
</div>

</div>
</div>
</div>
<!--Footer end-->