<?php
namespace capturePointer;
function lockerDb(){
	$db = array(
		'dsn'	=> 'mysql:host=localhost;dbname=website',
		'hostname' => 'localhost',
		'username' => 'root',
		'password' => '',
		'table' => 'locker',
		'prefix' => 'ws_',
		'char_set' => 'utf8',
		'dbcollat' => 'utf8_general_ci',
	);
	return $db;
}