
データ競合の防止の方法以下のような関数を使う。

* `pthread_mutex_lock`
* `atomic_fetch_add`
* `sem_wait`
