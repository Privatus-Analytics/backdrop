Privatus Analytics
==================

See real visitor numbers for your Backdrop site with no cookies, no cookie
pop-up and no bots. This module adds the Privatus Analytics script to the head
of your pages, with a simple settings page.

You want to know if people visit your site. But Google Analytics uses cookies,
so in many countries you must show a cookie pop-up first (the box that asks
visitors to say yes to cookies). Many people click no, so they never show up in
your numbers. Bots add fake visits on top.

The Privatus Analytics script sets no cookies and keeps no IP addresses. Bots
are filtered out before they are counted. So you see how many real people came,
which pages they looked at and where they came from, on one simple dashboard.

**Free plan, no card:** 10,000 events a month (an event is a page view or an
action you track) on up to 3 sites.
[Start free at privatusanalytics.com](https://privatusanalytics.com).

Requirements
------------

- A Privatus Analytics account. There is a free plan.
- Your site ID. It starts with `pa_`.

Installation
------------

- Install this module using the official Backdrop CMS instructions at
  https://docs.backdropcms.org/documentation/extend-with-modules.

- Visit the configuration page under Administration > Configuration > System >
  Privatus Analytics (admin/config/system/privatus-analytics).

- Enter your site ID and save. Nothing is loaded until you do.

- Optional: pick the modules you want and who is not tracked.

Settings
--------

- **Site ID:** Required. It must look like `pa_AbC123`.
- **Modules:** Extra things to measure.
  - Time on page and scroll depth (`engage`)
  - Outbound links, downloads, forms and 404 pages (`auto`)
  - Web Vitals, how fast pages load and react (`vitals`)
  - Click maps (`clicks`)
  - On-site surveys (`ask`)
- **Do not track administration pages:** On by default.
- **Do not track signed-in administrators:** On by default. Skips users with
  the "Do not track with Privatus Analytics" permission. Administrators have
  every permission, so they are skipped too.
- **Do not track users with these roles:** Pick roles to leave out.
- **Excluded paths:** Paths that are never counted.
- **More options:** Allowed domains, single-page app mode, manual page views,
  path masks, extra query parameters, canonical URL, hash routing and the
  script host. Each one maps to a `data-` attribute on the script. See the
  [tracker attributes](https://privatusanalytics.com/docs/tracker/attributes).

With a site ID and all modules on, every page gets this in its head:

```html
<script defer="defer" src="https://privatusstats.com/js/pa.js"
  data-site="pa_YOURSITEID" data-modules="engage,auto,vitals,clicks,ask">
</script>
```

Saving the settings clears the page cache, so visitors get the new tag
straight away.

External service
----------------

After you save a site ID, every page loads `https://privatusstats.com/js/pa.js`
in the visitor's browser. The script sends the page address (query parameters
are removed except campaign parameters and any you allow), the referrer, the
browser language, the screen width, the time zone name and event names to
`https://privatusstats.com/api/event`. The visitor's IP address is read in
memory to find a rough country and to count unique visits, and it is not
stored. The module never sends anything from your server.

- Terms of service: https://privatusanalytics.com/legal/terms
- Privacy policy: https://privatusanalytics.com/legal/privacy

Tests
-----

Turn on the Testing module, then run the tests from your Backdrop root:

```
php core/scripts/run-tests.sh --url http://your-site --class PrivatusAnalyticsTestCase
```

Issues
------

Bugs and feature requests should be reported in
[the Issue Queue](https://github.com/Privatus-Analytics/backdrop/issues).

Current Maintainers
-------------------

- [Privatus Analytics](https://github.com/Privatus-Analytics)

Credits
-------

- Written for Backdrop CMS by [Privatus Analytics](https://privatusanalytics.com).
- Based on the Privatus Analytics module for Drupal.

License
-------

This project is GPL v2 software.
See the LICENSE.txt file in this directory for complete text.
