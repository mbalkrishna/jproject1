<?php 
/*--get all active videos--*/
//$_nodes = node_load_multiple(array(), array('type' => 'video')); 

$vquery = new EntityFieldQuery();
$entities = $vquery->entityCondition('entity_type', 'node')
  ->propertyCondition('type', 'video')
  ->fieldCondition('field_video_status','value', 'Active')
  ->fieldOrderBy('field_view_order', 'value', 'ASC')
  ->execute();

$_nodes = node_load_multiple(array_keys($entities['node']));

$videos='';
foreach($_nodes as $video){
	
	$_video=$video->field_video_url;	
	$videoLink=$_video['und'][0]['value'];
	$videoMimeType=get_mime_content_type($videoLink);
	
	$videos.="{type: '{$videoMimeType}',  src:'{$videoLink}'},";
	
}; 
$videos=trim($videos,",");

/*--/get all active videos--*/

?>
<script>
jQuery(function() {
	var BV = new jQuery.BigVideo({preload: 'auto',doloop:true});
	var vids = [<?php echo $videos;?>];
	
	//vids.sort(function() { return 0.5 - Math.random() }); // random order on load
	BV.init();
	BV.showPlaylist(vids,{ambient:true,preload:true});
		
});
</script>