/* -*- Mode: IDL; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * https://wicg.github.io/entries-api/#idl-index
 */

interface FileSystemDirectoryEntry : FileSystemEntry {
    FileSystemDirectoryReader createReader();

    void getFile(optional USVString? path,
                 optional FileSystemFlags options = {},
                 optional FileSystemEntryCallback successCallback,
                 optional ErrorCallback errorCallback);

    void getDirectory(optional USVString? path,
                      optional FileSystemFlags options = {},
                      optional FileSystemEntryCallback successCallback,
                      optional ErrorCallback errorCallback);
};
