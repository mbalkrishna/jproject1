<?php
/*
Override user register form
*/
global $base_url,$theme_url;
?>

<div class="row">
<div class="col-lg-5 col-md-6 col-sm-7 col-xs-8 marginauto sign-up-box-sec">


<div class="signup-box signup-fb">
<h2>sign up</h2>
<p>
<?php
unset($form['field_first_name'][LANGUAGE_NONE][0]['value']['#title']);
unset($form['field_surname'][LANGUAGE_NONE][0]['value']['#title']);
unset($form['account']['mail']['#title']);
?>
<?php  print drupal_render($form['field_first_name']);?>
<?php  print drupal_render($form['field_surname']);?>
<?php  print drupal_render($form['account']['mail']);?>
<?php  print drupal_render($form['account']['pass']);?>
</p>

<?php
 print drupal_render($form['form_build_id']);
 print drupal_render($form['form_id']);
 print drupal_render($form['form_token']); 
?>
<p class="tc">By signing up, I agree to Where its at's <a href="<?php echo $base_url;?>/terms-conditions" target="_blank">terms & conditions</a> and <a href="<?php echo $base_url;?>/privacy-policy" target="_blank">privacy policy</a>.</p>
<p>
<?php
print drupal_render($form['actions']);
?>
</p>
</div>
</div>
</div>
<script>
$(function(){
	$("#user-register-form #edit-pass-pass1").attr("placeholder","password");
	$("#user-register-form #edit-pass-pass2").attr("placeholder","confirm password");		
});
</script>
