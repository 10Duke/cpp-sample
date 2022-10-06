#ifndef TENDUKE_LICENSING_TESTDEFAULTLICENSING_H
#define TENDUKE_LICENSING_TESTDEFAULTLICENSING_H


namespace tenduke { namespace tst { namespace licensing {


/** A command line demonstration of login + license checkout with "default" services.
 *  Uses auto-discovery to resolve OIDC-server configuration, client-configuration
 *  is set up in the code.
 *
 *  Login is done manually in browser: User opens the browser manually and this
 *  service tells what to do with the browser.
 */
void testDefaultLicensingWithAutoDiscovery();


}}}

#endif // TENDUKE_LICENSING_TESTDEFAULTLICENSING_H
