<?php
/*
Override user register form
*/
global $base_url,$theme_url;
?>

<div class="row">
<div class="col-lg-5 col-md-6 col-sm-7 col-xs-8 marginauto sign-up-box-sec">
<div class="signup-box signup-fb">
<h2>change password</h2>
<?php unset($form['current_pass']['#title']);?>
<p><?php print drupal_render($form['current_pass']);?></p>
<p><?php print drupal_render($form['account']['pass']);?></p>
<?php
 print drupal_render($form['form_build_id']);
 print drupal_render($form['form_id']);
 print drupal_render($form['form_token']); 
 print drupal_render($form['submit']);
?>
</div>

</div>
</div>
<script>
$(function(){
	$("#change-pwd-page-form #edit-pass-pass1").attr("placeholder","new password");
	$("#change-pwd-page-form #edit-pass-pass2").attr("placeholder","confirm password");
});
</script>


