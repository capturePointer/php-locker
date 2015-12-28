CREATE TABLE `ws_locker` (
 `name` varchar(255) NOT NULL DEFAULT '' COMMENT 'Primary Key: Unique name.',
 `value` varchar(255) NOT NULL DEFAULT '' COMMENT 'A value for the lock.',
 `expire` double NOT NULL COMMENT 'A Unix timestamp with microseconds indicating when the lock should expire.',
 PRIMARY KEY (`name`),
 KEY `value` (`value`),
 KEY `expire` (`expire`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='capturePointer Lock'