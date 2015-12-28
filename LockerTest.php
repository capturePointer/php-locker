<?php
require_once dirname(__FILE__) . "/Locker.inc";
if(!locker_acquire("lockName",3)){
	if(!locker_wait("lockName",3)){
		return false;
	}
}

//add your code


locker_release("lockName");