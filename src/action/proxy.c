// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2018-2019 Mutant Industries ltd.
#include <action/proxy.h>
#include <stddef.h>


#define _intercept(_proxy, _signal) action_proxy_signal_interceptor(_proxy)(action_owner(_proxy), &(_signal))

// -------------------------------------------------------------------------------------

// Action_proxy_t constructor
void action_proxy_register(Action_proxy_t *proxy, dispose_function_t dispose_hook,
        Action_t *target, bool persistent, signal_interceptor_t interceptor) {

    action_create(proxy, dispose_hook, proxy_trigger, interceptor);

    proxy->_target = target;
    proxy->_persistent = persistent;
}

// -------------------------------------------------------------------------------------

void proxy_trigger(Action_proxy_t *_this, signal_t signal) {

    // execute signal interceptor if set
    if (action_proxy_signal_interceptor(_this) && ! _intercept(_this, signal)) {
        return;
    }

    // just proxy trigger to target action
    action_trigger(_this->_target, signal);

    // if not persistent, proxy shall be triggered just once
    if ( ! _this->_persistent) {
        action_release(_this);
    }
}

