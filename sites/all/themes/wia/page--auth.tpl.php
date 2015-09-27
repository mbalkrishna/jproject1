<?php 
$wrapClasses='';
if(!empty($_SESSION['commerce_checkout_redirect_anonymous']) && arg(0)=='user') {//show normal bg for checkout login
	
	require_once "inc/header-inner.inc.php";
	$wrapClasses='checkout-login';
		
}else{?>	
	<?php require_once "inc/video-bg.inc.php";?>
	<?php require_once "inc/header.inc.php";?>
<?php
}?>


<!--Content Sec start-->
<div class="content-section login-content <?php echo $wrapClasses;?>">
<div class="v-align-box">
<div class="container">
<?php print render($page['header']); ?>
<?php if(isset($messages)){print $messages;} ?>
<?php print render($page['content']); ?>
<?php print render($page['footer']); ?>
</div>
</div>
</div>
<!--Content End start-->
<?php require_once "inc/footer.inc.php";?>