<?php
/**
 * @file
 * Settings form for Privatus Analytics.
 */

/**
 * Form constructor for the settings page.
 *
 * @see privatus_analytics_settings_form_validate()
 * @see privatus_analytics_settings_form_submit()
 */
function privatus_analytics_settings_form($form, &$form_state) {
  $config = config('privatus_analytics.settings');

  $form['intro'] = array(
    '#markup' => '<p>' . t('Privatus Analytics counts real visitors with no cookies, so you do not need a cookie pop-up for it. Once you save a site ID, visitors\' browsers load a small script from Privatus Analytics. Nothing is sent before that. <a href="@signup">Start free</a> | <a href="@terms">Terms of service</a> | <a href="@privacy">Privacy policy</a>', array(
      '@signup' => 'https://privatusanalytics.com',
      '@terms' => 'https://privatusanalytics.com/legal/terms',
      '@privacy' => 'https://privatusanalytics.com/legal/privacy',
    )) . '</p>',
  );

  $form['tracking'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tracking'),
  );
  $form['tracking']['site_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Site ID'),
    '#default_value' => $config->get('site_id'),
    '#size' => 30,
    '#maxlength' => 80,
    '#attributes' => array('placeholder' => 'pa_...'),
    '#description' => t('Your site ID, for example pa_AbC123. Find it in your Privatus Analytics dashboard under Site settings, then Setup. Leave it empty to turn tracking off.'),
  );
  $form['tracking']['modules'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Modules'),
    '#description' => t('Extra things to measure.'),
    '#options' => privatus_analytics_modules(),
    '#default_value' => (array) $config->get('modules'),
  );

  $form['exclusions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Who is not tracked'),
  );
  $form['exclusions']['exclude_admin_pages'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do not track administration pages'),
    '#default_value' => $config->get('exclude_admin_pages'),
  );
  $form['exclusions']['exclude_admins'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do not track signed-in administrators'),
    '#default_value' => $config->get('exclude_admins'),
    '#description' => t('Skips users with the "Do not track with Privatus Analytics" permission. Administrators have every permission, so they are skipped too.'),
  );
  $roles = user_roles(TRUE);
  $form['exclusions']['excluded_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Do not track users with these roles'),
    '#options' => $roles,
    '#default_value' => (array) $config->get('excluded_roles'),
  );
  $form['exclusions']['excluded_paths'] = array(
    '#type' => 'textfield',
    '#title' => t('Excluded paths'),
    '#default_value' => $config->get('excluded_paths'),
    '#maxlength' => 500,
    '#description' => t('Paths that are never counted, split by commas. A star matches anything. Example: /user/*,/private/*'),
  );

  $form['script'] = array(
    '#type' => 'fieldset',
    '#title' => t('More options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['script']['domains'] = array(
    '#type' => 'textfield',
    '#title' => t('Allowed domains'),
    '#default_value' => $config->get('domains'),
    '#maxlength' => 500,
    '#description' => t('Host names to count, split by commas. Visits on other host names, such as a test copy of your site, are left out. Example: example.com,www.example.com'),
  );
  $form['script']['spa'] = array(
    '#type' => 'select',
    '#title' => t('Single-page app mode'),
    '#options' => backdrop_map_assoc(privatus_analytics_spa_modes()),
    '#default_value' => $config->get('spa'),
    '#description' => t('How page changes without a full reload are found.'),
  );
  $form['script']['manual'] = array(
    '#type' => 'checkbox',
    '#title' => t('Manual page views'),
    '#default_value' => $config->get('manual'),
    '#description' => t('Turns off the automatic page view. You then call privatus.pageview() yourself.'),
  );
  $form['script']['path_masks'] = array(
    '#type' => 'textfield',
    '#title' => t('Path masks'),
    '#default_value' => $config->get('path_masks'),
    '#maxlength' => 500,
    '#description' => t('Rules in the form pattern->replacement, split by commas. Example: /user/*->/user/:id'),
  );
  $form['script']['extra_params'] = array(
    '#type' => 'textfield',
    '#title' => t('Extra query parameters'),
    '#default_value' => $config->get('extra_params'),
    '#maxlength' => 500,
    '#description' => t('Query parameters to keep, split by commas. Campaign (UTM) parameters are always kept. Example: page,lang'),
  );
  $form['script']['canonical'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use the canonical URL'),
    '#default_value' => $config->get('canonical'),
    '#description' => t('Record the rel=canonical address instead of the address bar.'),
  );
  $form['script']['hash'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hash routing'),
    '#default_value' => $config->get('hash'),
    '#description' => t('Count each #part of an address as its own page.'),
  );
  $form['script']['script_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Script host'),
    '#default_value' => $config->get('script_host') ? $config->get('script_host') : PRIVATUS_ANALYTICS_DEFAULT_HOST,
    '#description' => t('The address that serves the script. Keep the default unless you host your own copy.'),
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Form validation handler for privatus_analytics_settings_form().
 */
function privatus_analytics_settings_form_validate($form, &$form_state) {
  $site_id = trim((string) $form_state['values']['site_id']);
  if ($site_id !== '' && !privatus_analytics_valid_site_id($site_id)) {
    form_set_error('site_id', t('The site ID must look like pa_AbC123: pa_ and then only letters and numbers.'));
  }
  $host = trim((string) $form_state['values']['script_host']);
  if ($host !== '' && !privatus_analytics_valid_host($host)) {
    form_set_error('script_host', t('The script host must be an address such as https://privatusstats.com, with no path.'));
  }
}

/**
 * Form submission handler for privatus_analytics_settings_form().
 */
function privatus_analytics_settings_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $host = rtrim(trim((string) $values['script_host']), '/');
  $config = config('privatus_analytics.settings');
  $config->set('site_id', trim((string) $values['site_id']));
  $config->set('script_host', $host !== '' ? $host : PRIVATUS_ANALYTICS_DEFAULT_HOST);
  $config->set('modules', array_values(array_filter($values['modules'])));
  $config->set('exclude_admin_pages', (bool) $values['exclude_admin_pages']);
  $config->set('exclude_admins', (bool) $values['exclude_admins']);
  $config->set('excluded_roles', array_values(array_filter($values['excluded_roles'])));
  $config->set('spa', $values['spa']);
  $config->set('manual', (bool) $values['manual']);
  $config->set('canonical', (bool) $values['canonical']);
  $config->set('hash', (bool) $values['hash']);
  foreach (array('domains', 'excluded_paths', 'path_masks', 'extra_params') as $list) {
    $items = array_filter(array_map('trim', explode(',', (string) $values[$list])), 'strlen');
    $config->set($list, implode(',', $items));
  }
  $config->save();
  // Cached pages for visitors still hold the old tag.
  cache('page')->flush();
  backdrop_set_message(t('The configuration options have been saved.'));
}
