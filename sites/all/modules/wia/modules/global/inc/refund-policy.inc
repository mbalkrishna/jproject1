<?php
/*****
#action: refund_policy_form
#description: refund policy form
#author: PKS
#created: 05 Aug 2015
*****/

function refund_policy_form($form, &$form_state) {

	
	$form['#tree']=TRUE;
	
	
	$ruleCount=REFUND_POLICY_RULE_COUNT;
	
	$rfDB=get_refund_rules(1);
		
	for($r=0; $r<$ruleCount ; $r++){
	
		$rCount=$r+1;
		
		$status=0;
		$fromHours='';
		$toHours='';
		$amount=0;
		
		if(isset($rfDB[$rCount]['status']))
		$status=$rfDB[$rCount]['status'];
		
		if(isset($rfDB[$rCount]['from_hours']))
		$fromHours=$rfDB[$rCount]['from_hours'];
		
		if(isset($rfDB[$rCount]['to_hours']))
		$toHours=$rfDB[$rCount]['to_hours'];
		
		if(isset($rfDB[$rCount]['amount']))
		$amount=$rfDB[$rCount]['amount'];
		
		
		$form["refund_rule_{$r}"] = array(
			"#type" => "fieldset",
			"#title" => t("Refund Rule {$rCount}")
		);
		
		
		$form["refund_rule_{$r}"]["refund_policy"]["status"] = array(
			"#type" => "checkbox",
			"#title" => "Enable/Disable",
			"#default_value" =>$status 
		);	
		
		$form["refund_rule_{$r}"]["refund_policy"]["from_hours"] = array(
			"#title" => "From Hours",
			"#type" => "textfield",
			"#size" => 20,
			"#element_validate" => array("element_validate_integer"),
			"#default_value" => $fromHours
		);	
		
		$form["refund_rule_{$r}"]["refund_policy"]["to_hours"] = array(
			"#title" => "To Hours",
			"#type" => "textfield",
			"#size" => 20,
			"#element_validate" => array("element_validate_integer_positive"),
			"#default_value" => $toHours
		);	
		
		$form["refund_rule_{$r}"]["refund_policy"]["amount"] = array(
			"#title" => "Amount to Deduct",
			"#type" => "textfield",
			"#size" => 20,
			"#suffix" => "<span>Amount in %</span>",
			"#element_validate" => array("element_validate_number"),
			"#default_value" => $amount
		);	
	}	
	
	
	$form['submit'] = array(
		'#value' => 'Save',
		'#type' => 'submit'			
	);		
	
	$form['submit']['#attributes']['class'][]='btn save';		
	
	return $form;	
} 

/*****
#action: refund_policy_form_validate
#description: validate refund policy form.
#author: PKS
#created: 05 aug 2015
*****/
function refund_policy_form_validate($form, &$form_state) {
	
	
}


/*****
#action: refund_policy_form_submit
#description: save refund policy.
#author: PKS
#created: 5 Aug 2015
*****/
function refund_policy_form_submit($form, &$form_state) {
    	
	$ruleCount=REFUND_POLICY_RULE_COUNT;	
	$frmData=$form_state['values'];	
	
	for($r=0; $r<$ruleCount ; $r++){
			
		$policyNbr=$r+1;
				
		$fromHours=null;
		$toHours=null;
		$amount=0;
		$status=0;
		
		$frmFieldStatus=$frmData["refund_rule_{$r}"]["refund_policy"]['status'];
		if($frmFieldStatus && $frmFieldStatus==1){		
			
			$fromHours=$frmData["refund_rule_{$r}"]["refund_policy"]['from_hours'];
			$toHours=$frmData["refund_rule_{$r}"]["refund_policy"]['to_hours'];
			$amount=$frmData["refund_rule_{$r}"]["refund_policy"]['amount'];
			$status=$frmFieldStatus;		
		}		
		
		
		$srs=db_select('refund_policy','rp')
		->fields('rp',array('policy_nbr'))
		->condition('rp.policy_nbr',$policyNbr,'=')
		->execute()
		->fetch();
				
		if($srs && $policyNbr){
			
			$urs=db_update('refund_policy')->fields(array(		
			'from_hours' =>$fromHours,
			'to_hours'=>$toHours,
			'amount'=>$amount,
			'status'=>$status))
			->condition('policy_nbr',$policyNbr,'=')
			->execute();
			
		}else{		
			
			$irs=db_insert('refund_policy')->fields(array(
			'policy_nbr' => $policyNbr,
			'from_hours' =>$fromHours,
			'to_hours'=>$toHours,
			'amount'=>$amount,
			'status'=>$status))
			->execute();
		}
	}
	drupal_set_message("Refund policy updated successfully.");
	
} 