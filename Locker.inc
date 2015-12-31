<?php
namespace capturePointer;
include_once dirname(__FILE__) . '/LockerClass.inc';
function locker_acquire($name, $timeout = 30.0){
    $lock = \capturePointer\Locker::getInstance();
    $ret = $lock->acquire($name, $timeout);
    if(true === $ret){
        return true;
    }
    else{
        return false;
    }
}

function locker_wait($name,$delay = 30){
    $lock = \capturePointer\Locker::getInstance();
    $ret = $lock->wait($name, $delay);
    if(true === $ret){
        return true;
    }
    else{
        return false;
    }
}

function locker_release($name){
    $lock = \capturePointer\Locker::getInstance();
    $ret = $lock->release($name);
    if(true === $ret){
        return true;
    }
    else{
        return false;
    }
}