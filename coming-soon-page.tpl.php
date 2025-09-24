<!DOCTYPE html>
<html<?php print isset($language) ? ' lang="' . $language->langcode . '"' : ''; ?>>

  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title><?php print $title; ?></title>
    <style>
      body {
        margin: 0;
        padding: 0;
        font-family: 'Helvetica Neue', Arial, sans-serif;
        color: <?php print $text_color; ?>;
        display: flex;
        justify-content: center;
        align-items: center;
        min-height: 100vh;
        text-align: center;
        <?php if (!empty($background_image_url)): ?>background: url('<?php print $background_image_url; ?>') center center / cover no-repeat fixed;
        <?php else: ?>background-color: <?php print $background_color; ?>;
        <?php endif; ?>
      }

      .coming-soon-container {
        max-width: 600px;
        padding: 40px 20px;
        <?php if (!empty($background_image_url)): ?>background-color: rgba(<?php
                                                                            $rgb = sscanf($background_color, "#%02x%02x%02x");
                                                                            $opacity = $overlay_opacity / 100;
                                                                            print implode(', ', $rgb) . ', ' . $opacity;
                                                                            ?>);
        border-radius: 10px;
        box-shadow: 0 4px 20px rgba(0, 0, 0, 0.3);
        <?php endif; ?>
      }

      .coming-soon-title {
        font-size: 3em;
        font-weight: 300;
        margin-bottom: 20px;
        letter-spacing: 2px;
      }

      .coming-soon-message {
        font-size: 1.2em;
        line-height: 1.6;
        margin-bottom: 40px;
        opacity: 0.8;
      }

      .countdown-container {
        margin-top: 40px;
      }

      .countdown {
        display: flex;
        justify-content: center;
        gap: 30px;
        margin-top: 30px;
      }

      .countdown-item {
        text-align: center;
        min-width: 80px;
      }

      .countdown-number {
        font-size: 2.5em;
        font-weight: bold;
        display: block;
        line-height: 1;
      }

      .countdown-label {
        font-size: 0.9em;
        text-transform: uppercase;
        letter-spacing: 1px;
        opacity: 0.7;
        margin-top: 5px;
      }

      @media (max-width: 600px) {
        .coming-soon-title {
          font-size: 2em;
        }

        .countdown {
          gap: 15px;
        }

        .countdown-item {
          min-width: 60px;
        }

        .countdown-number {
          font-size: 1.8em;
        }
      }
    </style>
  </head>

  <body>
    <div class="coming-soon-container">
      <h1 class="coming-soon-title"><?php print $title; ?></h1>
      <div class="coming-soon-message"><?php print $message; ?></div>

      <?php if ($show_countdown && $launch_date): ?>
        <div class="countdown-container">
          <div class="countdown" id="countdown">
            <div class="countdown-item">
              <span class="countdown-number" id="days">00</span>
              <div class="countdown-label"><?php print t('Dies'); ?></div>
            </div>
            <div class="countdown-item">
              <span class="countdown-number" id="hours">00</span>
              <div class="countdown-label"><?php print t('Hores'); ?></div>
            </div>
            <div class="countdown-item">
              <span class="countdown-number" id="minutes">00</span>
              <div class="countdown-label"><?php print t('Minuts'); ?></div>
            </div>
            <div class="countdown-item">
              <span class="countdown-number" id="seconds">00</span>
              <div class="countdown-label"><?php print t('Segons'); ?></div>
            </div>
          </div>
        </div>

        <script>
          // Countdown timer
          var launchTime = '<?php print $launch_time; ?>' || '12:00';
          var launchDateTime = '<?php print $launch_date; ?>T' + launchTime + ':00';
          var launchDate = new Date(launchDateTime).getTime();

          function updateCountdown() {
            var now = new Date().getTime();
            var distance = launchDate - now;

            if (distance < 0 || isNaN(distance)) {
              document.getElementById('countdown').innerHTML = '<div style="font-size: 1.5em;"><?php print t("Ja estem aquí!"); ?></div>';
              return;
            }

            var days = Math.floor(distance / (1000 * 60 * 60 * 24));
            var hours = Math.floor((distance % (1000 * 60 * 60 * 24)) / (1000 * 60 * 60));
            var minutes = Math.floor((distance % (1000 * 60 * 60)) / (1000 * 60));
            var seconds = Math.floor((distance % (1000 * 60)) / 1000);

            document.getElementById('days').innerHTML = days.toString().padStart(2, '0');
            document.getElementById('hours').innerHTML = hours.toString().padStart(2, '0');
            document.getElementById('minutes').innerHTML = minutes.toString().padStart(2, '0');
            document.getElementById('seconds').innerHTML = seconds.toString().padStart(2, '0');
          }

          updateCountdown();
          setInterval(updateCountdown, 1000);
        </script>
      <?php elseif ($show_countdown): ?>
        <div class="countdown-container">
          <p>Comptador no disponible: data de llançament no configurada.</p>
        </div>
      <?php endif; ?>
    </div>
  </body>

  </html>
