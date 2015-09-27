<?php

/**
 * @file
 * Template to display a view as a table.
 *
 * - $title : The title of this group of rows.  May be empty.
 * - $header: An array of header labels keyed by field id.
 * - $caption: The caption for this table. May be empty.
 * - $header_classes: An array of header classes keyed by field id.
 * - $fields: An array of CSS IDs to use for each field id.
 * - $classes: A class or classes to apply to the table, based on settings.
 * - $row_classes: An array of classes to apply to each row, indexed by row
 *   number. This matches the index in $rows.
 * - $rows: An array of row items. Each row is an array of content.
 *   $rows are keyed by row number, fields within rows are keyed by field ID.
 * - $field_classes: An array of classes to apply to each field, indexed by
 *   field id, then row number. This matches the index in $rows.
 * @ingroup views_templates
 */
global $base_url, $theme_path;
?>
<table width="100%" border="0" cellspacing="0" cellpadding="0" class="cart-table">
  
  <thead>
  <tr>
    <td>image</td>
    <td>venue</td>
    <td>time slots</td>
    <td>amenities</td>
    <td align="right">amount</td>
    <td>&nbsp;</td>
        
  </tr>
  </thead>
  <tbody>  
    <?php foreach ($rows as $row_count => $row):
	
		$rowItem=commerce_line_item_load($row['line_item_id']);
		
		$pwrapper = entity_metadata_wrapper('commerce_product', $rowItem->commerce_product['und'][0]['product_id']);		
		?>
        <tr>          
        	<td width="10%">
            <?php 
			$fImg = $pwrapper->field_featured_image->value();
			 
			if($fImg && $fImg['uri']){
				
				$img = array(
				'style_name' => 'cart_thumbnail',
				'path' => $fImg['uri'],
				'alt' => $fImg['alt'],
				'title' => $fImg['title']			
				);
			
				print theme('image_style',$img);
			}else{
				echo '<img src="'.$base_url.'/'.$theme_path.'/images/no-image.jpg" width="50"/>';
			}                
			
			
			?>
            </td>
            <td width="30%"><span class="label-mob">venue</span><strong>
			<?php echo $row['line_item_title'];?>&nbsp;(<?php echo $pwrapper->field_room_name->value();?>)
            </strong>
            <?php if($pwrapper->field_full_address->value()){
				echo "<br/>".$pwrapper->field_full_address->value();
			}?>                
           </td>
            <td width="20%"><span class="label-mob">time slots</span><?php echo $rowItem->data['slot_details']['slot_descriptions'];?></td>
            
            <?php $amenities = $rowItem->data['req_data']['amenities'];
			$ame=get_amenites_by_aids(explode(",",$amenities),'list');
			?>
            
            <td width="20%"><span class="label-mob">amenities</span><?php echo $ame;?></td>
            <td align="right"><span class="label-mob">amount</span><?php echo CURRENCY_SYMBOL.$row['commerce_unit_price'];?></td>            
            <td align="right"><span class="label-mob">delete</span><?php echo $row['edit_delete'];?></td>
     
      </tr>
    <?php endforeach; ?>
  </tbody>
</table>