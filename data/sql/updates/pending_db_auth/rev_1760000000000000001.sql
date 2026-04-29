--
-- Premium/VIP: setdate/unsetdate -> StartTime/EndTime (datetime)
-- Keep columns: id, StartTime, EndTime, active
--

CREATE TABLE IF NOT EXISTS `account_premium` (
  `id` INT UNSIGNED NOT NULL COMMENT 'Account id',
  `setdate` INT UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Old Unix start (seconds)',
  `unsetdate` INT UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Old Unix end (seconds)',
  `active` TINYINT UNSIGNED NOT NULL DEFAULT 0 COMMENT '1 = active, 0 = inactive',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='Premium account status';

-- Add datetime columns for migration
ALTER TABLE `account_premium`
  ADD COLUMN `StartTime_new` DATETIME NOT NULL DEFAULT current_timestamp(),
  ADD COLUMN `EndTime_new`   DATETIME NOT NULL DEFAULT current_timestamp();

-- Migrate data from unix seconds
UPDATE `account_premium`
SET
  `StartTime_new` = IF(`setdate` > 0, FROM_UNIXTIME(`setdate`), current_timestamp()),
  `EndTime_new`   = IF(`unsetdate` > 0, FROM_UNIXTIME(`unsetdate`), current_timestamp());

-- Drop old unix columns and rename new ones
ALTER TABLE `account_premium`
  DROP COLUMN `setdate`,
  DROP COLUMN `unsetdate`,
  CHANGE COLUMN `StartTime_new` `StartTime` DATETIME NOT NULL DEFAULT current_timestamp(),
  CHANGE COLUMN `EndTime_new`   `EndTime`   DATETIME NOT NULL DEFAULT current_timestamp();

