<?php

/**
 * @file
 * Definition of views_plugin_style_jump_menu_enhanced.
 */

/**
 * Style plugin to render each item as a row in a table.
 *
 * @ingroup views_style_plugins
 */
class views_plugin_style_jump_menu_enhanced extends views_plugin_style_jump_menu {

  /**
   * {@inheritdoc}
   */
  public function option_definition() {
    $options = parent::option_definition();

    $options['parent_page_path'] = array('default' => '');
    $options['parent_page_label'] = array('default' => '', 'translatable' => TRUE);
    return $options;
  }

  /**
   * Render the given style.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $handlers = $this->display->handler->get_handlers('field');

    $form['parent_page_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Parent page path'),
      '#default_value' => $this->options['parent_page_path'],
    );

    $form['parent_page_label'] = array(
      '#type' => 'textfield',
      '#title' => t('Parent page label'),
      '#default_value' => $this->options['parent_page_label'],
      '#description' => t('The text that will appear as the the label of the selector element for index page you chose.'),
    );

  }

  /**
   * This is almost copy of views_plugin_style_jump_menu::render() with some changes for parent path.
   */
  public function render() {
    $sets = $this->render_grouping($this->view->result, $this->options['grouping']);

    // Turn this all into an $options array for the jump menu.
    $this->view->row_index = 0;
    $options = array();
    $paths = array();

    if (!empty($this->options['parent_page_path'])) {
        $path = $this->options['parent_page_path'];
      // Putting a '/' in front messes up url() so let's take that out
      // so users don't shoot themselves in the foot.
      $base_path = base_path();
      if (strpos($path, $base_path) === 0) {
        $path = drupal_substr($path, drupal_strlen($base_path));
      }
      $url_options = drupal_parse_url($path);
      $path = url($url_options['path'], $url_options);
      $parent_page_label = !empty($this->options['parent_page_label']) ? $this->options['parent_page_label'] : t('-All-');

      $key = md5($path . $parent_page_label) . "::" . $path;
      $options[$key] = $parent_page_label;

      $paths[$path] = $key;
    }

    foreach ($sets as $title => $records) {
      foreach ($records as $row_index => $row) {
        $this->view->row_index = $row_index;
        $path = strip_tags(decode_entities($this->get_field($this->view->row_index, $this->options['path'])));
        // Putting a '/' in front messes up url() so let's take that out
        // so users don't shoot themselves in the foot.
        $base_path = base_path();
        if (strpos($path, $base_path) === 0) {
          $path = drupal_substr($path, drupal_strlen($base_path));
        }

        // Use drupal_parse_url() to preserve query and fragment in case the
        // user wants to do fun tricks.
        $url_options = drupal_parse_url($path);

        $path = url($url_options['path'], $url_options);
        $field = strip_tags(decode_entities($this->row_plugin->render($row)));
        $key = md5($path . $field) . "::" . $path;
        if ($title) {
          $options[$title][$key] = $field;
        }
        else {
          $options[$key] = $field;
        }
        $url_options_for_path_without_fragment = $url_options;
        unset($url_options_for_path_without_fragment['fragment']);
        $paths[url($url_options['path'], $url_options_for_path_without_fragment)] = $key;
        $this->view->row_index++;
      }
    }
    unset($this->view->row_index);

    $default_value = '';
    if ($this->options['default_value']) {
      $lookup_options = array();
      // We need to check if the path is absolute or else language is not taken
      // in account.
      if (!empty($this->view->display[$this->view->current_display]->display_options['fields'][$this->options['path']]['absolute'])) {
        $lookup_options['absolute'] = TRUE;
      }
      $lookup_url = url($_GET['q'], $lookup_options);
      if (!empty($paths[$lookup_url])) {
        $default_value = $paths[$lookup_url];
      }
    }

    ctools_include('jump-menu');
    $settings = array(
      'hide' => $this->options['hide'],
      'button' => $this->options['text'],
      'title' => $this->options['label'],
      'choose' => $this->options['choose'],
      'inline' => $this->options['inline'],
      'default_value' => $default_value,
    );

    $form = drupal_get_form('ctools_jump_menu', $options, $settings);
    return $form;
  }

}
