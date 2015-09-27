<?php print render($page['header']); ?>
<?php global $base_url,$theme_path;?>
<!--Content Sec start-->
<?php
$siteLogoBG=variable_get('site_front_logo_bg');
if($siteLogoBG){
	$siteLogoBGPath=file_create_url($siteLogoBG);
	?>
	<div class="splash-page" style="background-image:url(<?php echo $siteLogoBGPath;?>);">
<?php }else{?>
	<div class="splash-page" style="background-image:url(<?php echo $base_url."/".$theme_path;?>/images/splash.jpg);">
<?php }?>

<div class="splash-content">

<div class="splash-logo-sec">

<div class="splash-logo">
<div class="splash-logo-symb">
<?php 
$siteLogo=variable_get('site_front_logo');
if($siteLogo){		
	$siteLogoPath=file_create_url($siteLogo);
	?>
	<img src="<?php echo $siteLogoPath;?>" alt="Where is it" />
	<?php 
}else{?>
	<img src="<?php echo $base_url."/".$theme_path;?>/images/splash-logo-symbol.png"  alt="Where is it"/>
<?php 
}?>
</div>

<div class="splash-logo-text">
<?php 
$siteLogoText=variable_get('site_front_logo_text');
if($siteLogoText){		
	$siteLogoTextPath=file_create_url($siteLogoText);
	?>
    <img src="<?php echo $siteLogoTextPath;?>" alt="Where is it"/>
	<?php 
}else{?>
  	<img src="<?php echo $base_url."/".$theme_path;?>/images/splash-logo-text.png"  alt="Where is it" />
<?php }?>

</div>

</div>
<a href="<?php echo $base_url;?>/space-search" class="btn left enter">enter</a>
</div>

</div>

</div>
</div>
<!--Content End start-->
<?php require_once "inc/footer.inc.php";?>