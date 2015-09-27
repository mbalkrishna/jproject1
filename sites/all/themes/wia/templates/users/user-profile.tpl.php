<?php
/*
Override user register form
*/
global $base_url,$theme_url,$user;
?>

<div class="row">
<div class="col-lg-5 col-md-6 col-sm-7 col-xs-8 marginauto sign-up-box-sec">
<?php 
$user_data = user_load($user->uid);
?>
<div class="signup-box signup-fb">
<h2>my account</h2>
<p><strong>first name</strong>:&nbsp;<?php print $user_data->field_first_name['und']['0']['value']; ?></p>
<p><strong>surname</strong>:&nbsp;<?php print $user_data->field_surname['und']['0']['value']; ?></p>
<p><strong>email</strong>:&nbsp;<?php print $user_data->mail; ?></p>

<a href="<?php echo $base_url;?>/user/<?php echo $user->uid;?>/change-password" class="change-password btn left">change password</a>
<a href="<?php echo $base_url;?>/user/<?php echo $user->uid;?>/edit" class="edit-profile btn left">edit profile</a>

</div>

</div>
</div>

