--
-- Premium/VIP: add chat color setting
--

ALTER TABLE `account_premium`
  ADD COLUMN `chat_text_color` TINYINT UNSIGNED NOT NULL DEFAULT 1;

