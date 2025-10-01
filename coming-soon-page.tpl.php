<?php

/**
 * @file coming-soon-page.tpl.php
 * Default theme implementation for the Coming Soon page.
 *
 * This template renders the Coming Soon page content when the module is enabled.
 *
 * Available variables:
 * - $title: The page title configured in the module settings.
 * - $message: The coming soon message, may contain HTML from nl2br().
 * - $show_countdown: Boolean indicating if the countdown timer should be displayed.
 * - $launch_date: The launch date in YYYY-MM-DD format.
 * - $launch_time: The launch time in HH:MM:SS format.
 * - $background_color: Hex color code for the background color.
 * - $background_image_url: URL to the background image, if configured.
 * - $overlay_opacity: Opacity percentage for the content overlay (0-100).
 * - $text_color: Hex color code for the text color.
 * - $font_family: Selected font family identifier.
 * - $title_size: Title font size in pixels.
 * - $message_size: Message font size in pixels.
 * - $language: The current language object.
 *
 * Helper variables:
 * - $classes: Array of CSS classes that can be used to style contextually.
 *
 * @see template_preprocess()
 * @see template_preprocess_coming_soon_page()
 * @see coming_soon_preprocess_page()
 *
 * @ingroup themeable
 */
?>
<div class="coming-soon-container">
  <h1 class="coming-soon-title"><?php print $title; ?></h1>
  <div class="coming-soon-message"><?php print $message; ?></div>

  <?php if ($show_countdown && $launch_date): ?>
    <div class="countdown-container">
      <div class="countdown" id="countdown">
        <div class="countdown-item">
          <span class="countdown-number" id="days">00</span>
          <div class="countdown-label"><?php print t('Days'); ?></div>
        </div>
        <div class="countdown-item">
          <span class="countdown-number" id="hours">00</span>
          <div class="countdown-label"><?php print t('Hours'); ?></div>
        </div>
        <div class="countdown-item">
          <span class="countdown-number" id="minutes">00</span>
          <div class="countdown-label"><?php print t('Minutes'); ?></div>
        </div>
        <div class="countdown-item">
          <span class="countdown-number" id="seconds">00</span>
          <div class="countdown-label"><?php print t('Seconds'); ?></div>
        </div>
      </div>
    </div>
  <?php endif; ?>
</div>
