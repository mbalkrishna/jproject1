<?php
/*
Override user reset password form
*/
global $base_url,$theme_url;
?>

<div class="row">
<div class="col-lg-5 col-md-6 col-sm-7 col-xs-8 marginauto sign-up-box-sec">
<div class="signup-box signup-fb">
<h2>reset password</h2>
<p class="agree-tc"><?php print render($form['message']); ?></p>
<?php
 print drupal_render($form['form_build_id']);
 print drupal_render($form['form_id']);
 print drupal_render($form['form_token']); 
 print drupal_render($form['actions']);
?>
</div>
</div>
</div>



