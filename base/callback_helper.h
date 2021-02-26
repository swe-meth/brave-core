/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BASE_CALLBACK_HELPER_H_
#define BRAVE_BASE_CALLBACK_HELPER_H_

#include <utility>

#include "base/bind.h"
#include "base/callback.h"
#include "base/memory/scoped_refptr.h"
#include "base/sequenced_task_runner.h"
#include "base/task/post_task.h"

namespace base {

// Bind the OnceCallback and gurantee that it will be ran on certain sequence.
// Normally we want callback to be ran on current
// thread(base::SequencedTaskRunnerHandle::Get()) so no matter callback.Run() is
// called on which sequence, it will alwasy be ran on the one we pass in.
template <typename R, typename... Args>
base::OnceCallback<R(Args...)> BindOnceCallbackToSequence(
    scoped_refptr<base::SequencedTaskRunner> task_runner,
    base::OnceCallback<R(Args...)> callback) {
  return base::BindOnce(
      [](scoped_refptr<base::SequencedTaskRunner> task_runner,
         base::OnceCallback<R(Args...)> callback, Args... args) {
        if (task_runner->RunsTasksInCurrentSequence())
          std::move(callback).Run(std::forward<Args>(args)...);
        else
          task_runner->PostTask(
              FROM_HERE,
              base::BindOnce(std::move(callback), std::forward<Args>(args)...));
      },
      task_runner, std::move(callback));
}

}  // namespace base

#endif  // BRAVE_BASE_CALLBACK_HELPER_H_
