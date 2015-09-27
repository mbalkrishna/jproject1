<?php
/*
Override user register form
*/
global $base_url,$theme_url;
?>

<div class="row">
<div class="col-lg-5 col-md-6 col-sm-7 col-xs-8 marginauto sign-up-box-sec">

<div class="signup-box signup-fb">
<h2>edit profile</h2>
<p>
<?php 
//unset title dynamically
unset($form['field_first_name'][LANGUAGE_NONE][0]['value']['#title']);
unset($form['field_surname'][LANGUAGE_NONE][0]['value']['#title']);
unset($form['account']['mail']['#title']);
?>
<?php  print drupal_render($form['field_first_name']);?>
<?php  print drupal_render($form['field_surname']);?>
<?php  print drupal_render($form['account']['mail']);?>
</p>

<?php
 print drupal_render($form['form_build_id']);
 print drupal_render($form['form_id']);
 print drupal_render($form['form_token']);
 print drupal_render($form['actions']);
?>
</p>

</div>

</div>
</div>

