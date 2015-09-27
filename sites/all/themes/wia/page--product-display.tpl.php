<?php require_once "inc/header.inc.php";?>

<!--Content Sec start-->
<div class="content-section  asset-details">
<?php print render($page['header']); ?>
<?php if(isset($messages)){
	//print $messages;
	$_SESSION['global']['messages']=$messages;
} ?>
<?php print render($page['content']); ?>
<?php print render($page['footer']); ?>
</div>
<!--Content End start-->
<?php require_once "inc/footer.inc.php";?>