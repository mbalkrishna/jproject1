<?php
require_once "functions.php";

/**
 * Override of theme_breadcrumb().
 */
function wia_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];

  if (!empty($breadcrumb)) {
    // Provide a navigational heading to give context for breadcrumb links to
    // screen-reader users. Make the heading invisible with .element-invisible.
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';

    $output .= '<div class="breadcrumb">' . implode(' › ', $breadcrumb) . '</div>';
    return $output;
  }
}

/**
 * Override or insert variables into the maintenance page template.
 */
function wia_preprocess_maintenance_page(&$vars) {
  // While markup for normal pages is split into page.tpl.php and html.tpl.php,
  // the markup for the maintenance page is all in the single
  // maintenance-page.tpl.php template. So, to have what's done in
  // wia_preprocess_html() also happen on the maintenance page, it has to be
  // called here.
  wia_preprocess_html($vars);
}

/**
 * Override or insert variables into the html template.
 */
function wia_preprocess_html(&$vars) {
  // Toggle fixed or fluid width.
  if (theme_get_setting('wia_width') == 'fluid') {
    $vars['classes_array'][] = 'fluid-width';
  }
  // Add conditional CSS for IE6.
  drupal_add_css(path_to_theme() . '/fix-ie.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'lt IE 7', '!IE' => FALSE), 'preprocess' => FALSE));
}

/**
 * Override or insert variables into the html template.
 */
function wia_process_html(&$vars) {
  // Hook into color.module
  if (module_exists('color')) {
    _color_html_alter($vars);
  }
}


/**
 * Override or insert JS.
 */
function wia_js_alter(&$javascript) {	


	$defaultJQ=$javascript['misc/jquery.js'];
	unset($javascript['misc/jquery.js']);
	
	if((arg(0)=='asset' && arg(1)=='add') || arg(0)=='imce'){//assign default js for these pages
		$javascript['misc/jquery.js']=$defaultJQ;
	}
	
	unset($javascript['misc/ui/jquery.ui.core.min.js']);
	unset($javascript['misc/ui/jquery.ui.widget.min.js']);
	unset($javascript['misc/ui/jquery.ui.position.min.js']);
	unset($javascript['misc/ui/jquery.ui.resizable.min.js']);
	unset($javascript['misc/ui/jquery.ui.dialog.min.js']);
	unset($javascript['misc/ui/jquery.ui.mouse.min.js']);
	unset($javascript['misc/ui/jquery.ui.button.min.js']);
	unset($javascript['misc/ui/jquery.ui.draggable.min.js']);
	
}

/**
 * Override or insert CSS.
 */
function wia_css_alter(&$css) {
	
	unset($css['misc/ui/jquery.ui.core.css']);
	unset($css['misc/ui/jquery.ui.theme.css']);
	unset($css['modules/system/system.theme.css']);
	unset($css['misc/ui/jquery.ui.button.css']);
	unset($css['misc/ui/jquery.ui.dialog.css']);
	unset($css['misc/ui/jquery.ui.resizable.css']);
	unset($css['modules/system/system.menus.css']);
	unset($css['sites/all/libraries/fontawesome/css/font-awesome.css']);
	unset($css['sites/all/modules/date/date_popup/themes/datepicker.1.7.css']);
	
	
	
}


/**
 * Override or insert variables into the page template.
 */
function wia_preprocess_page(&$vars) {
	
	// Move secondary tabs into a separate variable.
	$vars['tabs2'] = array(
	'#theme' => 'menu_local_tasks',
	'#secondary' => $vars['tabs']['#secondary'],
	);
	unset($vars['tabs']['#secondary']);
	
	if (isset($vars['main_menu'])) {
		$vars['primary_nav'] = theme('links__system_main_menu', array(
		  'links' => $vars['main_menu'],
		  'attributes' => array(
			'class' => array('links', 'inline', 'main-menu'),
		  ),
		  'heading' => array(
			'text' => t('Main menu'),
			'level' => 'h2',
			'class' => array('element-invisible'),
		  )
		));
	}
	else {
		$vars['primary_nav'] = FALSE;
	}
	if (isset($vars['secondary_menu'])) {
		$vars['secondary_nav'] = theme('links__system_secondary_menu', array(
		  'links' => $vars['secondary_menu'],
		  'attributes' => array(
			'class' => array('links', 'inline', 'secondary-menu'),
		  ),
		  'heading' => array(
			'text' => t('Secondary menu'),
			'level' => 'h2',
			'class' => array('element-invisible'),
		  )
		));
		}
	else {
		$vars['secondary_nav'] = FALSE;
	}
	
	// Prepare header.
	$site_fields = array();
	if (!empty($vars['site_name'])) {
		$site_fields[] = $vars['site_name'];
	}
	if (!empty($vars['site_slogan'])) {
		$site_fields[] = $vars['site_slogan'];
	}
	$vars['site_title'] = implode(' ', $site_fields);
	if (!empty($site_fields)) {
		$site_fields[0] = '<span>' . $site_fields[0] . '</span>';
	}
	$vars['site_html'] = implode(' ', $site_fields);
	
	// Set a variable for the site name title and logo alt attributes text.
	$slogan_text = $vars['site_slogan'];
	$site_name_text = $vars['site_name'];
	$vars['site_name_and_slogan'] = $site_name_text . ' ' . $slogan_text;
	
	/*--modified by PKS @ 6Jun 15--*/
	//space serach template
	if (in_array(arg(0),array('space-search'))){	
    	$vars['theme_hook_suggestions'][] = 'page__space_search';		
  	}
	if (in_array(arg(0),array('space-search-result'))){	
    	$vars['theme_hook_suggestions'][] = 'page__space_search_result';		
  	}

	if (isset($vars['node']->type)) {
     	if($vars['node']->type=='product_display')
        $vars['theme_hook_suggestions'][] = 'page__product_display';
    }	
	
	if (arg(0)=='user'){	
    	$vars['theme_hook_suggestions'][] = 'page__auth';		
  	}
	
	if (in_array(arg(0),array('asset')) && in_array(arg(1),array('add','edit'))){	
    	$vars['theme_hook_suggestions'][] = 'page__add_space';		
  	}
	
	
	if (arg(0)=='checkout' && arg(2)=='complete'){	
    	$vars['theme_hook_suggestions'][] = 'page__checkout_complete';		
  	}
	
	//assign temeplate for 404 & 403 Page
	$status = drupal_get_http_header("status");  
	if($status == '403 Forbidden') {
		$vars['theme_hook_suggestions'][] = 'page__403';
	}
	if($status == '404 Not Found') {
		$vars['theme_hook_suggestions'][] = 'page__404';
	}
	
	
	/*--/modified by PKS @ 6Jun 15--*/		
	drupal_set_title(strtolower(htmlspecialchars_decode(drupal_get_title())));	
	
}

/**
 * Override or insert variables into the node template.
 */
function wia_preprocess_node(&$vars) {
	$vars['submitted'] = $vars['date'] . ' — ' . $vars['name'];
}

/**
 * Override or insert variables into the comment template.
 */
function wia_preprocess_comment(&$vars) {
	$vars['submitted'] = $vars['created'] . ' — ' . $vars['author'];
}

/**
 * Override or insert variables into the block template.
 */
function wia_preprocess_block(&$vars) {
	$vars['title_attributes_array']['class'][] = 'title';
	$vars['classes_array'][] = 'clearfix';
}

/**
 * Override or insert variables into the page template.
 */
function wia_process_page(&$vars) {
	// Hook into color.module
	if (module_exists('color')) {
		_color_page_alter($vars);
	}	
}

/**
 * Override or insert variables into the region template.
 */
function wia_preprocess_region(&$vars) {
	if ($vars['region'] == 'header') {
		$vars['classes_array'][] = 'clearfix';
	}
}


/*****
#action: wia_theme
#description: overide templates themes.
#author: Pawan
#created: 22 June 2015
*****/

function wia_theme() {
	
	$items = array();
	$items['user_login'] = array(
		'render element' => 'form',
		'path' => drupal_get_path('theme', 'wia') . '/templates/users',
		'template' => 'user-login',
		'preprocess functions' => array(
			'wia_preprocess_user_login'
		),
	);
	
	$items['user_pass'] = array(
		'render element' => 'form',
		'path' => drupal_get_path('theme', 'wia') . '/templates/users',
		'template' => 'user-pass',
		'preprocess functions' => array(
			'wia_preprocess_user_pass'
		),
	);
	
	
	$items['user_register_form'] = array(
		'render element' => 'form',
		'path' => drupal_get_path('theme', 'wia') . '/templates/users',
		'template' => 'user-register-form',
		'preprocess functions' => array(
			'wia_preprocess_user_register_form'
		),
	);
	
	
	$items['user_profile_form'] = array(
		'render element' => 'form',
		'path' => drupal_get_path('theme', 'wia') . '/templates/users',
		'template' => 'user-profile-form',
		'preprocess functions' => array(
			'wia_preprocess_user_profile_form'
		),
	);
	
	$items['user_profile'] = array(
		'render element' => 'content',
		'path' => drupal_get_path('theme', 'wia') . '/templates/users',
		'template' => 'user-profile',
		'preprocess functions' => array(
			'wia_preprocess_user_profile'
		),
	);	
	
	$items['change_pwd_page_form'] = array(
		'render element' => 'form',
		'path' => drupal_get_path('theme', 'wia') . '/templates/users',
		'template' => 'change-pwd-page-form',
		'preprocess functions' => array(
			'wia_preprocess_change_pwd_page_form'
		),
	);
	
	$items['change_pwd_page_user_pass_reset'] = array(
		'render element' => 'form',
		'path' => drupal_get_path('theme', 'wia') . '/templates/users',
		'template' => 'change-pwd-page-user-pass-reset',
		'preprocess functions' => array(
			'wia_preprocess_change_pwd_page_user_pass_reset'
		),
	);
	
	return $items;
}


/*****
#action: wia_preprocess_image
#description: overide image height and width.
#author: Pawan
#created: 22 June 2015
*****/
function wia_preprocess_image(&$variables) {
	
  $attributes = &$variables['attributes'];
  foreach (array('width', 'height') as $key) {
    unset($attributes[$key]);
    unset($variables[$key]);
  }
}


function wia_preprocess_user_profile(&$variables) {
	drupal_set_title('my account');
}

/**
 * Returns HTML for an individual file upload widget.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: A render element representing the widget.
 *
 * @ingroup themeable
 */
function wia_file_widget($variables) {
  
	$element = $variables['element'];
	$output = '';
	
	// The "form-managed-file" class is required for proper Ajax functionality.
	$output.='<div class="row">';
	$output .= '<div class="col-sm-4 file-widget form-managed-file clearfix">';
	if ($element['fid']['#value'] != 0) {
	// Add the file size after the file name.
	$element['filename']['#markup'] .= ' <span class="file-size">(' . format_size($element['#file']->filesize) . ')</span> ';
	}  
	$output .= drupal_render_children($element);
	$output .= '</div>';
	$output .= '</div>';
	
	return $output;
}

/**
 * Returns HTML for a group of file upload widgets.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: A render element representing the widgets.
 *
 * @ingroup themeable
 */
function wia_file_widget_multiple($variables) {	
	
	$element = $variables['element'];
	
	// Special ID and classes for draggable tables.
	$weight_class = $element['#id'] . '-weight';
	$table_id = $element['#id'] . '-table';
	
	// Build up a table of applicable fields.
	$headers = array();
	$headers[] = t('File information');
	if ($element['#display_field']) {
	$headers[] = array(
	'data' => t('Display'),
	'class' => array('checkbox'),
	);
	}
	$headers[] = t('Weight');
	$headers[] = t('Operations');
	
	// Get our list of widgets in order (needed when the form comes back after
	// preview or failed validation).
	$widgets = array();
	foreach (element_children($element) as $key) {
		$widgets[] = &$element[$key];
	}
	usort($widgets, '_field_sort_items_value_helper');
	
	$rows = array();
	
	$out='';
	$out.='<div class="row">';
	
	foreach ($widgets as $key => &$widget) {
		// Save the uploading row for last.
		if ($widget['#file'] == FALSE) {
		$widget['#title'] = $element['#file_upload_title'];
		$widget['#description'] = $element['#file_upload_description'];
		continue;
		}
		
		// Delay rendering of the buttons, so that they can be rendered later in the
		// "operations" column.
		$operations_elements = array();
		foreach (element_children($widget) as $sub_key) {
			if (isset($widget[$sub_key]['#type']) && $widget[$sub_key]['#type'] == 'submit') {
			hide($widget[$sub_key]);
			$operations_elements[] = &$widget[$sub_key];
			}
		}
		
		// Delay rendering of the "Display" option and the weight selector, so that
		// each can be rendered later in its own column.
		if ($element['#display_field']) {
			hide($widget['display']);
		}
		hide($widget['_weight']);
		
		// Render everything else together in a column, without the normal wrappers.
		$widget['#theme_wrappers'] = array();
		$information = drupal_render($widget);
		
		// Render the previously hidden elements, using render() instead of
		// drupal_render(), to undo the earlier hide().
		$operations = '';
		foreach ($operations_elements as $operation_element) {
			$operations .= render($operation_element);
		}
		
		$display = '';
		if ($element['#display_field']) {
			unset($widget['display']['#title']);
			$display = array(
			'data' => render($widget['display']),
			'class' => array('checkbox'),
			);
		}
		$widget['_weight']['#attributes']['class'] = array($weight_class);
		$weight = render($widget['_weight']);
		
		// Arrange the row with all of the rendered columns.
		$row = array();
		$row[] = $information;
		if ($element['#display_field']) {
			$row[] = $display;
		}
		$row[] = $weight;
		$row[] = $operations;
		$rows[] = array(
		'data' => $row,
		'class' => isset($widget['#attributes']['class']) ? array_merge($widget['#attributes']['class'], array('draggable')) : array('draggable'),
		);
		
		$out .= '<div class="col-sm-4 col-xs-6 file-widget form-managed-file clearfix">';
		$out.= $information;
		$out.= $display;
		$out.= $operations;
		//$out.= $weight;
		$out.= '</div>';
	}
	
	$out.= '</div>';
	
	
	drupal_add_tabledrag($table_id, 'order', 'sibling', $weight_class);
	
	$output = '';
	//$output = empty($rows) ? '' : theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => array('id' => $table_id)));  
	$output=$out;
	$output .= drupal_render_children($element);
	return $output;
}


/**
 * Returns HTML for an image field widget.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: A render element representing the image field widget.
 *
 * @ingroup themeable
 */
function wia_image_widget($variables) {
	
	$element = $variables['element'];
	$output = '';
	
	$output .= '<div class="single-image image-widget form-managed-file clearfix">';
	
	if (isset($element['preview'])) {
		$output .= '<div class="image-preview">';
		$output .= drupal_render($element['preview']);
		$output .= '</div>';
	}
	
	$output .= '<div class="image-widget-data">';
	if ($element['fid']['#value'] != 0) {
		$element['filename']['#markup'] .= ' <span class="file-size">(' . format_size($element['#file']->filesize) . ')</span> ';
	}
	
	if ($element['fid']['#value'] == 0) {
		
		$output .= '<span class="file-desc">&nbsp;</span>';
		$output .= '<a class="add-image btn" href="javascript:void(0);" onclick="proceed_file_upload(\''.$element['#id'].'\',this)">add photo</a>';
	}
	
	$output .= '<div class="image-field">';
	$output .= drupal_render_children($element);
	$output .= '</div>';
	
	$output .= '</div>';
	$output .= '</div>';
	return $output;
}