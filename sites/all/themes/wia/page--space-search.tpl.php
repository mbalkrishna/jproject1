<?php require_once "inc/video-bg.inc.php";?>
<?php require_once "inc/header.inc.php";?>
<!--Content Sec start-->
<div class="content-section">
<div class="container text-right">

<?php 
$searchHeadline=variable_get('search_page_headline');
if($searchHeadline){
	echo $searchHeadline;
}
?>
<div class="search-section">
<?php print render($page['header']); ?>
<?php if(isset($messages)){print $messages;} ?>
<?php print render($page['content']); ?>
<?php print render($page['footer']); ?>

</div>
</div>
</div>
<!--Content End start-->
<?php require_once "inc/footer.inc.php";?>