<?php require_once "inc/header-inner.inc.php";?>
<!--Content Sec start-->
<div class="content-section search-content">
<div class="thank-you-box">
<h1>Thank you for booking with us!</h1>
</div>
<div class="container margintop40 paddingbottom60">
<?php print render($title_prefix); ?>
<?php if ($title): ?>
<h1<?php print $tabs ? ' class="with-tabs"' : '' ?>><?php print $title; ?></h1>
<?php endif; ?>
<?php print render($title_suffix); ?>
<?php print render($page['header']); ?>
<?php if(isset($messages)){print $messages;} ?>
<?php print render($page['content']); ?>
<?php print render($page['footer']); ?>
</div>
</div>
<!--Content End start-->
<?php require_once "inc/footer.inc.php";?>