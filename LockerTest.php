<?php
require_once dirname(__FILE__) . "/Locker.inc";
if(!\capturePointer\locker_acquire("lockName",10.0)){
	if(!\capturePointer\locker_wait("lockName",3)){
		return false;
	}
	else if(!\capturePointer\locker_acquire("lockName",10.0)){
		return false;
	}
}

//add your code


\capturePointer\locker_release("lockName");