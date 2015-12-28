<?php
namespace capturePointer;
require_once dirname(__FILE__) . "/LockerDb.inc";
class Locker
{
	
    private static $__instance;

    private $__link;

    private $__db;

    private $__sessionId;
	
    private function __construct()
    {
		$this->__db = \capturePointer\lockerDb();
		$this->__link = new \PDO($this->__db['dsn'],$this->__db['username'],$this->__db['password']);
		$this->__sessionId = session_id();
		if(empty($this->__sessionId)) {
			session_start();		
			$this->__sessionId = session_id();
		}
    }
    
    public static function getInstance(){
        if(!(self::$__instance instanceof self)){
            self::$__instance = new self;
        }
        return self::$__instance;
    }
	
    public function __destruct()
    {
		$this->__releaseExpired();
    }
	
    public function acquire($name, $timeout = 30.0)
    {
		if(empty($this->__sessionId)) {
			return false;
		}
		$this->__releaseExpired();
		$sql = "SELECT * FROM " . $this->__db['prefix'] . $this->__db['table'] . " WHERE `name`=:name AND expire >= :expire LIMIT 1";
		$stmt = $this->__link->prepare($sql);
		$stmt->bindParam(':name', $name, \PDO::PARAM_STR);
		$expire = microtime(TRUE);
		$stmt->bindParam(':expire', $expire, \PDO::PARAM_INT);
		$stmt->execute();
		$rows = $stmt->fetch(\PDO::FETCH_OBJ);
		if(empty($rows)){
                    $sql = "INSERT INTO `" . $this->__db['prefix'] . $this->__db['table'] .
                    "`(`name`,`value`,`expire`) VALUES 
                    ( :name,:value,:expire )";
                    $stmt = $this->__link->prepare($sql);
                    $expire = microtime(TRUE) + $timeout;
                    $stmt->execute(array(':name' => $name, ':value' => md5($this->__sessionId), ':expire' => $expire));
                    $count = $stmt->rowCount();
                    if($count > 0){
                            return true;
                    }
		}
		return false;
    }
	
    public function wait($name,$delay = 30)
    {
		if(empty($this->__sessionId)) {
			return false;
		}
		$delay = (int) $delay * 1000000;
		$sleep = 25000;
		while($delay > 0){
			usleep($sleep);
			$delay = $delay - $sleep;
			$sleep = min(500000, $sleep + 25000, $delay);
			$sql = "SELECT * FROM " . $this->__db['prefix'] . $this->__db['table'] . " WHERE `name`=:name AND expire >= :expire LIMIT 1";
			$stmt = $this->__link->prepare($sql);
			$stmt->bindParam(':name', $name, \PDO::PARAM_STR);
			$expire = microtime(TRUE);
			$stmt->bindParam(':expire', $expire, \PDO::PARAM_INT);
			$stmt->execute();
			$rows = $stmt->fetch(\PDO::FETCH_OBJ);
			if(empty($rows)){
				return true;
			}
		}
		return false;
    }
	
    public function release($name)
    {
		if(empty($this->__sessionId)) {
			return false;
		}
		$sql = "DELETE FROM `" . $this->__db['prefix'] . $this->__db['table'] .
		"`WHERE `name`=:name AND `value`=:value";
		$stmt = $this->__link->prepare($sql);
		$stmt->execute(array(':name' => $name, ':value' => md5($this->__sessionId)));
		$count = $stmt->rowCount();
		if($count > 0){
			return true;
		}
		return false;
    }
	
    private function __releaseExpired()
    {
		$sql = "DELETE FROM `" . $this->__db['prefix'] . $this->__db['table'] .
		"`WHERE `expire` <= :expire";
		$stmt = $this->__link->prepare($sql);
		$stmt->execute(array(':expire' => microtime(TRUE)));
		$count = $stmt->rowCount();
		if($count > 0){
			return true;
		}
		return false;
    }
}