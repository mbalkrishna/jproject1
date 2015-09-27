<?php require_once "inc/header-inner.inc.php";?>

<!--Content Sec start-->
<div class="content-section search-content">
<?php print render($page['header']); ?>
<?php if(isset($messages)){print $messages;} ?>
<?php print render($page['content']); ?>
<?php print render($page['footer']); ?>
</div>
<!--Content End start-->

<style>.wia-front div.messages {  background-position: 8px 8px;  background-repeat: no-repeat;  border: 1px solid;  margin: 6px 0;  padding: 10px;  max-width: 50%;  margin: 0;  margin-bottom: 15px;
  margin-top: 15px;  vertical-align: middle;  margin-left: 43.5%;}</style>


<?php require_once "inc/footer.inc.php";?>