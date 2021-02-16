// Copyright (c) 2021 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'

import {
  StyledDialogWrapper,
  StyledDialog,
  DialogTitle,
  StyledCloseIcon,
  StyledInputLabel,
  StyledInput,
  StyledButtonsContainer,
  StyledButton,
} from './style'
import { getLocale } from '../../../../common/locale'
import CloseIcon from './assets/close-icon'

interface Props {
  showEditTopSite: boolean
  textDirection: string
  onClose: () => void
  onSave: (title: string, url: string) => void
}

export default class EditTopSite extends React.PureComponent<Props, {}> {
  editTopSiteDialogRef: React.RefObject<any>
  nameInputRef: React.RefObject<any>
  urlInputRef: React.RefObject<any>
  constructor (props: Props) {
    super(props)
    this.editTopSiteDialogRef = React.createRef()
    this.nameInputRef = React.createRef()
    this.urlInputRef = React.createRef()
  }

  componentDidMount () {
    document.addEventListener('mousedown', this.handleClickOutside)
    document.addEventListener('keydown', this.onKeyPressSettings)
  }

  componentWillUnmount () {
    document.removeEventListener('mousedown', this.handleClickOutside)
  }

  onKeyPressSettings = (event: KeyboardEvent) => {
    if (event.key === 'Escape') {
      this.props.onClose()
    }
  }

  handleClickOutside = (event: Event) => {
    if (
      this.editTopSiteDialogRef &&
      this.editTopSiteDialogRef.current &&
      !this.editTopSiteDialogRef.current.contains(event.target)
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
