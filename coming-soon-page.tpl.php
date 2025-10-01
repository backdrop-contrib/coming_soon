<!DOCTYPE html>
<html<?php print isset($language) ? ' lang="' . $language->langcode . '"' : ''; ?>>

  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title><?php print $title; ?></title>
    <?php print $styles; ?>
  </head>

  <body class="coming-soon-page">
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

    <?php print $scripts; ?>
  </body>

  </html>
