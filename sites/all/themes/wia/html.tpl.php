<?php
/**
 * @file
 * Default theme implementation to display the basic html structure of a single
 * Drupal page.
 *
 * Variables:
 * - $css: An array of CSS files for the current page.
 * - $language: (object) The language the site is being displayed in.
 *   $language->language contains its textual representation.
 *   $language->dir contains the language direction. It will either be 'ltr' or 'rtl'.
 * - $rdf_namespaces: All the RDF namespace prefixes used in the HTML document.
 * - $grddl_profile: A GRDDL profile allowing agents to extract the RDF data.
 * - $head_title: A modified version of the page title, for use in the TITLE
 *   tag.
 * - $head_title_array: (array) An associative array containing the string parts
 *   that were used to generate the $head_title variable, already prepared to be
 *   output as TITLE tag. The key/value pairs may contain one or more of the
 *   following, depending on conditions:
 *   - title: The title of the current page, if any.
 *   - name: The name of the site.
 *   - slogan: The slogan of the site, if any, and if there is no title.
 * - $head: Markup for the HEAD section (including meta tags, keyword tags, and
 *   so on).
 * - $styles: Style tags necessary to import all CSS files for the page.
 * - $scripts: Script tags necessary to load the JavaScript files and settings
 *   for the page.
 * - $page_top: Initial markup from any modules that have altered the
 *   page. This variable should always be output first, before all other dynamic
 *   content.
 * - $page: The rendered page content.
 * - $page_bottom: Final closing markup from any modules that have altered the
 *   page. This variable should always be output last, after all other dynamic
 *   content.
 * - $classes String of classes that can be used to style contextually through
 *   CSS.
 *
 * @see template_preprocess()
 * @see template_preprocess_html()
 * @see template_process()
 *
 * @ingroup themeable
 */
 ?>
<?php global $base_url,$theme_path;?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN"
  "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" version="XHTML+RDFa 1.0" dir="<?php print $language->dir; ?>"<?php print $rdf_namespaces; ?>>

<head profile="<?php print $grddl_profile; ?>">
<?php print $head; ?>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title><?php print $head_title; ?></title>
<?php print $styles; ?>


<link href="<?php echo $base_url."/".$theme_path;?>/css/bootstrap.css" rel="stylesheet"/>
<link href="<?php echo $base_url."/".$theme_path;?>/css/styles.css" rel="stylesheet"/>
<link href="<?php echo $base_url."/".$theme_path;?>/css/menu.css" rel="stylesheet"/>
<link href="<?php echo $base_url."/".$theme_path;?>/css/menu-effect.css" rel="stylesheet"/>
<link href="<?php echo $base_url."/".$theme_path;?>/css/font-awesome.css" rel="stylesheet"/>

<link href="<?php echo $base_url."/".$theme_path;?>/css/dd.css" rel="stylesheet"/>
<link href="<?php echo $base_url."/".$theme_path;?>/css/fonts.css" type="text/css" rel="stylesheet"/>
<link href="<?php echo $base_url."/".$theme_path;?>/css/jquery.mobile.datepicker.css" rel="stylesheet"/>
<!--
<link href="<?php //echo $base_url."/".$theme_path;?>/css/jquery.mobile.datepicker.theme.css" rel="stylesheet"/>
-->
<link href="<?php echo $base_url."/".$theme_path;?>/css/build.css" rel="stylesheet"/>
<link href="<?php echo $base_url."/".$theme_path;?>/css/dev.css" rel="stylesheet"/>

<script src="http://ajax.googleapis.com/ajax/libs/jquery/2.0.3/jquery.min.js"></script>
<?php print $scripts; ?>
<script src="http://code.jquery.com/ui/1.11.4/jquery-ui.js"></script>
<script type="text/javascript" src="<?php echo $base_url."/".$theme_path;?>/js/menu.js"></script>

<link href="<?php echo $base_url."/".$theme_path;?>/css/bigvideo.css" rel="stylesheet"/>
<script src="<?php echo $base_url."/".$theme_path;?>/js/video.js/video.js"></script>  
<script src="<?php echo $base_url."/".$theme_path;?>/js/bigvideo.js"></script>

<script type="text/javascript" src="<?php echo $base_url."/".$theme_path;?>/js/bootstrap.min.js"></script>
<script src="<?php echo $base_url."/".$theme_path;?>/js/datepicker.js"></script>
<script src="<?php echo $base_url."/".$theme_path;?>/js/jquery.dd.js"></script>
<!--
<script src="<?php echo $base_url."/".$theme_path;?>/js/jquery.maskedinput.js"></script>
-->

<script>
$ = jQuery;
JS_BASE_URL='<?php echo $base_url;?>';
</script>
 
<script src="<?php echo $base_url."/".$theme_path;?>/js/wia.js"></script>

  
</head>

<body class="wia-front <?php print $classes; ?>" <?php print $attributes;?>>
<div id="skip-link">
<a href="#main-content" class="element-invisible element-focusable"><?php print t('Skip to main content'); ?></a>
</div>
<?php print $page_top; ?>
<?php print $page; ?>
<?php print $page_bottom; ?>

<!--[if lt IE 10]>
<script src="<?php echo $base_url."/".$theme_path;?>/js/jquery.placeholder.js"></script>
<script type="text/javascript">
// To test the @id toggling on password inputs in browsers that don't support changing an input's @type dynamically (e.g. Firefox 3.6 or IE), uncomment this:
// $.fn.hide = function() { return this; }
// Then uncomment the last rule in the <style> element (in the <head>).
$(function() {
// Invoke the plugin
$('input, textarea').placeholder({customClass:'my-placeholder'});
// That's it, really.
// Now display a message if the browser supports placeholder natively
});
</script>
<![endif]-->      
</body>
</html>
