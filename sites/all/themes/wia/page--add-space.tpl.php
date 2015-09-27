<?php require_once "inc/header-inner.inc.php";?>
<!--Content Sec start-->
<div class="content-section search-content">
<div class="container margintop40 paddingbottom60">
<?php print render($page['header']); ?>
<?php if(isset($messages)){print $messages;} ?>
<?php print render($page['content']); ?>
<?php print render($page['footer']); ?>
</div>
</div>
<!--Content End start-->
<?php require_once "inc/footer.inc.php";?>