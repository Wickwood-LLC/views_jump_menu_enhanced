<?php

/**
 * Implements hook_views_plugins().
 */
function views_jump_menu_enhanced_views_plugins() {
  //$js_path = drupal_get_path('module', 'ctools') . '/js';
  $plugins = array();

  if (module_invoke('ctools', 'api_version', '1.3')) {
    // $plugins['style']['jump_menu_summary'] = array(
    //   'title' => t('Jump menu'),
    //   'help' => t('Puts all of the results into a select box and allows the user to go to a different page based upon the results.'),
    //   'handler' => 'views_plugin_style_summary_jump_menu',
    //   'theme' => 'views_view_summary_jump_menu',
    //   'type' => 'summary',
    // // only shows up as a summary style.
    //   'uses options' => TRUE,
    //   'help topic' => 'style-summary-jump-menu',
    // );
    $plugins['style']['jump_menu_enhanced'] = array(
      'title' => t('Jump menu (enhanced)'),
      'help' => t('Puts all of the results into a select box and allows the user to go to a different page based upon the results.'),
      'handler' => 'views_plugin_style_jump_menu_enhanced',
      'theme' => 'views_view_jump_menu',
      'uses row plugin' => TRUE,
      'uses fields' => TRUE,
      'uses options' => TRUE,
      'type' => 'normal',
      'help topic' => 'style-jump-menu',
    );
  }

  return $plugins;
}