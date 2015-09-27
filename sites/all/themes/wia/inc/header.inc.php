<?php global $base_url,$theme_path;?>
<!--Header sec start-->    
<div id="header">
<div class="container">





<div class="row">

<div class="col-md-4 col-sm-3">

<a href="<?php echo $base_url;?>">

<?php 
$siteLogo=variable_get('site_logo');
if($siteLogo){		
	$siteLogoPath=file_create_url($siteLogo);
	?>
	<img src="<?php echo $siteLogoPath;?>"  /><?php 
}else{?>
	<img src="<?php echo $base_url."/".$theme_path;?>/images/logo.png"  />
<?php 
}?>

</a>




<?php /*?><div class="logo-sec">
<a href="<?php echo $base_url;?>">

<?php 
$siteLogo=variable_get('site_logo');
if($siteLogo){		
	$siteLogoPath=file_create_url($siteLogo);
	?>
	<span class="logo-icon"><img src="<?php echo $siteLogoPath;?>"  /></span><?php 
}else{?>
	<span class="logo-icon"><img src="<?php echo $base_url."/".$theme_path;?>/images/logo-icon.png"  /></span>
<?php 
}?>


<?php 
$siteLogoText=variable_get('site_logo_text');
if($siteLogoText){		
	$siteLogoTextPath=file_create_url($siteLogoText);
	?>
	<span class="logo-text"><img src="<?php echo $siteLogoTextPath;?>"  alt="Where is it" /></span>
<?php 
}else{?>
	<span class="logo-text"><img src="<?php echo $base_url."/".$theme_path;?>/images/logo-text.png"  alt="Where is it" /></span>
<?php }?>
</a>
</div><?php */?>





</div>


<div class="col-md-8 col-sm-9">

<?php
global $user;
$userLink = drupal_get_path_alias('user/' . $user->uid);
$userId=$user->uid;
//top nav
echo "<ul class='top-nav'>";
if ($user->uid != 0) {	
	echo "<li><a href='{$base_url}/my-bookings/{$userId}'>My Bookings</a></li>";
	echo "<li><a href='{$base_url}/user/logout'>Log out</a></li>";	
	echo "<li class='my-account'><a href='{$base_url}/{$userLink}'>My Account</a></li>";
}
$ccount=get_current_cart_item();
if($ccount>0){
	echo "<li class='cart'><a href='{$base_url}/cart'>&nbsp;<span class='cart-count'>{$ccount}</span></a></li>";
}else{
	echo "<li class='cart'><a href='{$base_url}/cart'>&nbsp;</a></li>";
}
echo "</ul>"; 
?>

<div class="cssmenu cl-effect-3">
<?php
print theme('links', array('links' => menu_navigation_links('main-menu'),'attributes' => array('class' => false)));
?>
</div>
</div>

</div><!--/row-->

</div>
</div>
<!--Header sec end--> 