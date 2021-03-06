/* This Source Code Form is subject to the terms of the Mozilla Public
 * License. v. 2.0. If a copy of the MPL was not distributed with this file.
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import styled from 'styled-components'

export const StyledListContent = styled('div')<{}>`
  padding: 0 24px;
`

export const StyledSitesNum = styled('div')<{}>`
  padding: 24px;
`

export const StyledDisabledContent = styled('div')<{}>`
  padding: 0px 5px;
`

export const StyledHeading = styled('span')<{}>`
  font-size: 22px;
  font-weight: normal;
  letter-spacing: 0;
  line-height: 28px;
`

export const StyledSitesLink = styled('a')<{}>`
  float: right;
  color: #4C54D2;
  font-size: 13px;
  letter-spacing: 0;
`

export const StyledText = styled('p')<{}>`
  color: #838391;
  font-size: 14px;
  font-family: ${p => p.theme.fontFamily.body};
  font-weight: 300;
  letter-spacing: 0;
  line-height: 28px;
`

export const StyledTotalContent = styled('div')<{}>`
  position: relative;
  padding-right: 25px;

  @media (max-width: 366px) {
    top: 11px;
  }
`

export const StyledWalletOverlay = styled('div')<{}>`
  display: flex;
  position: fixed;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  background: ${p => p.theme.color.modalOverlayBackground};
  align-items: center;
  z-index: 999;
  justify-content: center;
`

export const StyledWalletWrapper = styled('div')<{}>`
  height: 90vh;
  overflow-y: scroll;
  width: 90%;
  margin-top: 40px;
`

export const StyledWalletClose = styled('div')<{}>`
  top: 15px;
  right: 15px;
  position: fixed;
  color: ${p => p.theme.color.subtleExclude};
  width: 25px;
`

export const StyledMobileSettingsContainer = styled('div')`
  display: flex;
  flex-direction: column;
`
