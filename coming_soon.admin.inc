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

  $form['excluded_paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Excluded paths'),
    '#description' => t('Specify paths that should not show the coming soon page. Enter one path per line. Use * as a wildcard. Leave empty to show coming soon on all pages. Admin pages and user/login are always excluded.'),
    '#default_value' => $config->get('excluded_paths'),
    '#rows' => 5,
  );

  $form['show_site_name'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the Site Name'),
    '#default_value' => $config->get('show_site_name'),
    '#description' => t('When enabled, Site Name will also be shown.'),
  );

  $form['page_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Page title'),
    '#description' => t('The title to display on the coming soon page.'),
    '#default_value' => $config->get('page_title'),
    '#required' => TRUE,
  );

  $form['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#description' => t('The message to display to visitors.'),
    '#default_value' => $config->get('message'),
    '#rows' => 4,
    '#required' => TRUE,
  );

  $form['countdown'] = array(
    '#type' => 'fieldset',
    '#title' => t('Countdown settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['countdown']['show_countdown'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show countdown timer'),
    '#description' => t('Display a countdown timer until the launch date.'),
    '#default_value' => $config->get('show_countdown'),
  );

  $form['countdown']['launch_datetime'] = array(
    '#type' => 'html_datetime',
    '#title' => t('Launch date and time'),
    '#default_value' => array(
      'date' => $config->get('launch_datetime.date'),
      'time' => $config->get('launch_datetime.time'),
    ),
  );

  $form['styling'] = array(
    '#type' => 'fieldset',
    '#title' => t('Styling'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['styling']['background_color'] = array(
    '#type' => 'color',
    '#title' => t('Background color'),
    '#description' => t('Background color for the coming soon page (used if no background image is set, or as overlay color if image is set).'),
    '#default_value' => $config->get('background_color'),
  );

  $form['styling']['background_image'] = array(
    '#type' => 'managed_file',
    '#title' => t('Background image'),
    '#description' => t('Upload a background image for the coming soon page.'),
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
    '#default_value' => $config->get('overlay_opacity'),
    '#min' => 0,
    '#max' => 100,
    '#step' => 5,
    '#field_suffix' => '<span class="range-value" data-for="overlay_opacity">' . $config->get('overlay_opacity') . '</span>%',
  );

  $form['styling']['text_color'] = array(
    '#type' => 'color',
    '#title' => t('Text color'),
    '#description' => t('Text color for the coming soon page.'),
    '#default_value' => $config->get('text_color'),
  );

  $form['styling']['font_family'] = array(
    '#type' => 'select',
    '#title' => t('Font family'),
    '#description' => t('Choose the font family for the text.'),
    '#default_value' => $config->get('font_family'),
    '#options' => array(
      'helvetica' => t('Helvetica Neue (Default)'),
      'georgia' => t('Georgia (Serif)'),
      'times' => t('Times New Roman (Classic)'),
      'arial' => t('Arial (Sans-serif)'),
      'verdana' => t('Verdana (Clean)'),
      'courier' => t('Courier New (Monospace)'),
      'impact' => t('Impact (Bold)'),
      'trebuchet' => t('Trebuchet MS (Modern)'),
    ),
  );

  $form['styling']['title_size'] = array(
    '#type' => 'range',
    '#title' => t('Title size'),
    '#description' => t('Size of the main title.'),
    '#default_value' => $config->get('title_size'),
    '#min' => 24,
    '#max' => 72,
    '#step' => 4,
    '#field_suffix' => '<span class="range-value" data-for="title_size">' . $config->get('title_size') . '</span>px',
  );

  $form['styling']['message_size'] = array(
    '#type' => 'range',
    '#title' => t('Message size'),
    '#description' => t('Size of the message text.'),
    '#default_value' => $config->get('message_size'),
    '#min' => 14,
    '#max' => 28,
    '#step' => 1,
    '#field_suffix' => '<span class="range-value" data-for="message_size">' . ($config->get('message_size') ?: 19) . '</span>px',
  );

  $form['permissions_note'] = array(
    '#type' => 'item',
    '#title' => t('Permissions'),
    '#markup' => t('Users with the "Bypass coming soon page" permission will be able to access the site normally. You can configure this at !link.', array(
      '!link' => l(t('admin/config/people/permissions'), 'admin/config/people/permissions', array('fragment' => 'module-coming_soon')),
    )),
  );

  $form['#attached']['js'][] = backdrop_get_path('module', 'coming_soon') . '/js/coming_soon.js';

  $form = system_settings_form($form);
  return $form;
}

/**
 * Validation handler for coming_soon_admin_form().
 */
function coming_soon_admin_form_validate($form, &$form_state) {
  // Handle empty excluded_paths - ensure it saves as empty string instead of NULL
  if (empty($form_state['values']['excluded_paths'])) {
    $form_state['values']['excluded_paths'] = '';
  } else {
    // Clean up the paths - remove extra whitespace
    $paths = array_filter(array_map('trim', explode("\n", $form_state['values']['excluded_paths'])));
    $form_state['values']['excluded_paths'] = implode("\n", $paths);
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
}
