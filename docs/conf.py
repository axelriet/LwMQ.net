# -- Project information

project = 'LwMQ'
copyright = 'Axel Rietschin Software Development, LLC'
author = 'Axel Rietschin'
release = '0.1'

extensions = [ 'sphinx_rtd_theme' ]
html_static_path = ['_static']
html_theme = "sphinx_rtd_theme"

html_theme_options = {
    'analytics_id': 'G-VW7BZSSV48',
    'analytics_anonymize_ip': False,
    'logo_only': False,
    'prev_next_buttons_location': 'bottom',
    'style_external_links': False,
    'vcs_pageview_mode': '',
    'style_nav_header_background': '#2980B9',
    'flyout_display': 'hidden',
    'version_selector': True,
    'language_selector': True,
    # Toc options
    'collapse_navigation': True,
    'sticky_navigation': True,
    'navigation_depth': 4,
    'includehidden': True,
    'titles_only': False
}

html_css_files = [
    'css/custom.css',
]