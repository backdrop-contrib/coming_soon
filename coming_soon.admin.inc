<?php

/**
 * @file
 * Administrative functions for Coming Soon module.
 */

/**
 * Admin form for coming soon settings.
 */
function coming_soon_admin_form($form, &$form_state) {
  $config = config('coming_soon.settings');

  $form['#config'] = 'coming_soon.settings';

  $form['enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable coming soon page'),
    '#description' => t('When enabled, visitors will see the coming soon page instead of the normal site.'),
    '#default_value' => $config->get('enabled'),
  );

  $form['page_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Page title'),
    '#description' => t('The title to display on the coming soon page.'),
    '#default_value' => $config->get('page_title') ?: t('Coming Soon'),
    '#required' => TRUE,
  );

  $form['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#description' => t('The message to display to visitors.'),
    '#default_value' => $config->get('message') ?: t('We are working hard to bring you something amazing. Stay tuned!'),
    '#rows' => 4,
    '#required' => TRUE,
  );

  $form['countdown'] = array(
    '#type' => 'fieldset',
    '#title' => t('Countdown settings'),
    '#collapsible' => TRUE,
    '#collapsed' => !$config->get('show_countdown'),
  );

  $form['countdown']['show_countdown'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show countdown timer'),
    '#description' => t('Display a countdown timer until the launch date.'),
    '#default_value' => $config->get('show_countdown'),
  );

  $saved_date = $config->get('launch_date');
  $default_date = array(
    'year' => date('Y', strtotime('+1 month')),
    'month' => date('n', strtotime('+1 month')),
    'day' => date('j', strtotime('+1 month')),
  );

  if (!empty($saved_date) && is_string($saved_date)) {
    $date_parts = explode('-', $saved_date);
    if (count($date_parts) == 3) {
      $default_date = array(
        'year' => (int)$date_parts[0],
        'month' => (int)$date_parts[1],
        'day' => (int)$date_parts[2],
      );
    }
  }

  $form['countdown']['launch_date'] = array(
    '#type' => 'date',
    '#title' => t('Launch date'),
    '#description' => t('The expected launch date for the countdown timer.'),
    '#default_value' => $default_date,
    '#states' => array(
      'visible' => array(
        ':input[name="show_countdown"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['countdown']['launch_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Launch time'),
    '#description' => t('The expected launch time in HH:MM format (24-hour format).'),
    '#default_value' => $config->get('launch_time') ?: '12:00',
    '#size' => 10,
    '#maxlength' => 5,
    '#states' => array(
      'visible' => array(
        ':input[name="show_countdown"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['styling'] = array(
    '#type' => 'fieldset',
    '#title' => t('Styling'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['styling']['background_color'] = array(
    '#type' => 'color',
    '#title' => t('Background color'),
    '#description' => t('Background color for the coming soon page (used if no background image is set, or as overlay color if image is set).'),
    '#default_value' => $config->get('background_color') ?: '#ffffff',
  );

  $form['styling']['background_image'] = array(
    '#type' => 'managed_file',
    '#title' => t('Background image'),
    '#description' => t('Upload a background image for the coming soon page. If set, this will override the background color.'),
    '#default_value' => $config->get('background_image') ? $config->get('background_image') : NULL,
    '#upload_location' => 'public://coming_soon/',
    '#upload_validators' => array(
      'file_validate_extensions' => array('png gif jpg jpeg webp'),
      'file_validate_image_resolution' => array('1920x1080', '320x240'),
    ),
  );

  $form['styling']['overlay_opacity'] = array(
    '#type' => 'range',
    '#title' => t('Overlay opacity'),
    '#description' => t('Controls the transparency of the content area overlay when using a background image (0 = fully transparent, 100 = fully opaque).'),
    '#default_value' => $config->get('overlay_opacity') ?: 90,
    '#min' => 0,
    '#max' => 100,
    '#step' => 5,
    '#field_suffix' => '<span id="opacity-value">' . ($config->get('overlay_opacity') ?: 90) . '</span>%',
    '#states' => array(
      'visible' => array(
        ':input[name="files[background_image]"]' => array('filled' => TRUE),
      ),
    ),
    '#attached' => array(
      'js' => array(
        '(function($) {
          $(document).ready(function() {
            $("input[name=\'overlay_opacity\']").on("input", function() {
              $("#opacity-value").text($(this).val());
            });
          });
        })(jQuery);' => array('type' => 'inline'),
      ),
    ),
  );

  $form['styling']['text_color'] = array(
    '#type' => 'color',
    '#title' => t('Text color'),
    '#description' => t('Text color for the coming soon page.'),
    '#default_value' => $config->get('text_color') ?: '#333333',
  );

  $form['permissions_note'] = array(
    '#type' => 'item',
    '#title' => t('Permissions'),
    '#markup' => t('Users with the "Bypass coming soon page" permission will be able to access the site normally. You can configure this at !link.', array(
      '!link' => l(t('admin/config/people/permissions'), 'admin/config/people/permissions', array('fragment' => 'module-coming_soon')),
    )),
  );

  $form = system_settings_form($form);
  return $form;
}

/**
 * Validation handler for coming_soon_admin_form().
 */
function coming_soon_admin_form_validate($form, &$form_state) {
  // Validate time format
  $launch_time = $form_state['values']['launch_time'];
  if (!empty($launch_time) && !preg_match('/^([01]?[0-9]|2[0-3]):[0-5][0-9]$/', $launch_time)) {
    form_set_error('launch_time', t('Launch time must be in HH:MM format (e.g., 14:30).'));
  }

  // Handle background image upload
  if (!empty($form_state['values']['background_image'])) {
    $fid = is_array($form_state['values']['background_image']) ? $form_state['values']['background_image'][0] : $form_state['values']['background_image'];
    $file = file_load($fid);
    if ($file) {
      // Make the file permanent
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      file_usage_add($file, 'coming_soon', 'config', 1);

      // Store just the file ID for the configuration
      $form_state['values']['background_image'] = $fid;
    } else {
      $form_state['values']['background_image'] = 0;
    }
  } else {
    $form_state['values']['background_image'] = 0;
  }

  // Convert date array to string format for automatic saving
  if (isset($form_state['values']['launch_date']) && is_array($form_state['values']['launch_date'])) {
    $date = $form_state['values']['launch_date'];
    if (!empty($date['year']) && !empty($date['month']) && !empty($date['day'])) {
      $form_state['values']['launch_date'] = sprintf(
        '%04d-%02d-%02d',
        $date['year'],
        $date['month'],
        $date['day']
      );
    } else {
      $form_state['values']['launch_date'] = '';
    }
  }
}
