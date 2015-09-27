<?php
/*
Override user login page
*/
global $base_url,$theme_url;
?>
<div class="row">

<div class="col-sm-9 marginauto">

<div class="login-box">
<div class="col-lg-8 col-md-7 col-sm-6 login-form">
<h2>login</h2>
<?php
unset($form['name']['#title']);
unset($form['pass']['#title']);
?>

<?php print drupal_render($form['name']);?>
<?php print drupal_render($form['pass']);?>
<?php
print drupal_render($form['form_build_id']);
print drupal_render($form['form_id']);
print drupal_render($form['form_token']);
print drupal_render($form['actions']);
?>
<p><a href="<?php echo $base_url;?>/user/password" class="forgot-password">Have you forgotten your password?</a></p>

</div>
<div class=" col-lg-4 col-md-5 col-sm-6 signup-fb">
<h2>not registered?</h2>

<!--
<p><a href="#"><img src="<?php echo $base_url."/".$theme_url;?>/images/fb-login.png"  /></a></p>
<p>or</p>
-->

<p><a href="<?php echo $base_url;;?>/user/register" class="btn btn-signup">sign up with email</a></p>
<p class="agree-tc">By signing up, I agree to Where its at's <a href="<?php echo $base_url;?>/terms-conditions" target="_blank">terms & conditions</a> and <br /><a href="<?php echo $base_url;?>/privacy-policy" target="_blank">privacy policy</a>.</p>

</div>
</div>
</div>
</div>

