-- Free 1-day VIP for first login (one-time per account)
CREATE TABLE IF NOT EXISTS `account_premium_free_day` (
    `id` INT UNSIGNED NOT NULL COMMENT 'Account id',
    `claimed_at` INT UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Unix time when free VIP day was granted',
    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='One-time free VIP day per account';

