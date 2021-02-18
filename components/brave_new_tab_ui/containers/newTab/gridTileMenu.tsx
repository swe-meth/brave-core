// Copyright (c) 2021 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'

import { getLocale } from '../../../common/locale'

interface Props {
  onEdit: (title: string, url: string) => void
  onClose: () => void
}

export default class GridTileMenu extends React.PureComponent<Props, {}> {
  tileMenuRef: React.RefObject<any>
  constructor (props: Props) {
    super(props)
    this.tileMenuRef = React.createRef()
  }

  componentDidMount () {
    document.addEventListener('mousedown', this.handleClickOutside)
  }

  componentWillUnmount () {
    document.removeEventListener('mousedown', this.handleClickOutside)
  }

  handleClickOutside = (event: Event) => {
    if (
      this.tileMenuRef &&
      this.tileMenuRef.current &&
      !this.tileMenuRef.current.contains(event.target)
    ) {
      this.props.onClose()
    }
  }

  saveNewTopSite = () => {
    this.props.onSave(this.nameInputRef.current.value,
                      this.urlInputRef.current.value)
  }

  render () {
    const {
      showEditTopSite,
      textDirection,
      onClose,
    } = this.props

    if (!showEditTopSite) {
      return null
    }

    return (
      <StyledDialogWrapper textDirection={textDirection}>
        <StyledDialog
          innerRef={this.editTopSiteDialogRef}
          textDirection={textDirection}
        >
          <DialogTitle>
            {getLocale('addTopSiteDialogTitle')}
          </DialogTitle>
          <StyledCloseIcon onClick={onClose}>
            <CloseIcon/>
          </StyledCloseIcon>
          <StyledInputLabel>
            {getLocale('addTopSiteDialogNameLabel')}
          </StyledInputLabel>
          <StyledInput
            autoFocus
            innerRef={this.nameInputRef}
            type="text"
            placeholder={getLocale('addTopSiteDialogNameInputPlaceHolder')}
          />
          <StyledInputLabel>
            {getLocale('addTopSiteDialogURLLabel')}
          </StyledInputLabel>
          <StyledInput
            innerRef={this.urlInputRef}
            type="url"
            placeholder={getLocale('addTopSiteDialogURLInputPlaceHolder')}
          />
          <StyledButtonsContainer>
            <StyledButton
              text={getLocale('addTopSiteDialogCancelButtonLabel')}
              level={'secondary'}
              size={'small'}
              onClick={onClose}
            />
            <StyledButton
              text={getLocale('addTopSiteDialogSaveButtonLabel')}
              level={'primary'}
              type={'accent'}
              size={'small'}
              onClick={this.saveNewTopSite}
            />
          </StyledButtonsContainer>
        </StyledDialog>
      </StyledDialogWrapper>
    )
  }
}
