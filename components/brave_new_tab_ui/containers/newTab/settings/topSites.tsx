// Copyright (c) 2020 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.*/

import * as React from 'react'

import {
  SettingsRow,
  SettingsText,
  StyledTopSitesCustomizationSettings,
  StyledTopSitesCustomizationSettingsOption,
  StyledTopSitesCustomizationImageBorder,
  StyledTopSitesCustomizationImage,
  StyledTopSitesCustomizationOptionTitle,
  StyledTopSitesCustomizationOptionDesc
} from '../../../components/default'
import { Toggle } from '../../../components/toggle'

import { getLocale } from '../../../../common/locale'

import favoritesSelectedLight from './assets/favorites-selected.png'
import favoritesUnselectedLight from './assets/favorites-unselected.png'
import frecencySelectedLight from './assets/frecency-selected.png'
import frecencyUnselectedLight from './assets/frecency-unselected.png'

import favoritesSelectedDark from './assets/favorites-selected-dark.png'
import favoritesUnselectedDark from './assets/favorites-unselected-dark.png'
import frecencySelectedDark from './assets/frecency-selected-dark.png'
import frecencyUnselectedDark from './assets/frecency-unselected-dark.png'

interface Props {
  toggleShowTopSites: () => void
  showTopSites: boolean
  customLinksEnabled: boolean
  setMostVisitedSettings: (show: boolean, customize: boolean) => void
}

class TopSitesSettings extends React.PureComponent<Props, {}> {
  onClickFavorites = () => {
    this.props.setMostVisitedSettings(true, true)
  }

  onClickFrecency = () => {
    this.props.setMostVisitedSettings(true, false)
  }

  render () {
    const {
      toggleShowTopSites,
      showTopSites,
      customLinksEnabled
    } = this.props

    const favoritesSelected = showTopSites && customLinksEnabled
    const frecencySelected = showTopSites && !customLinksEnabled
    return (
      <div>
        <SettingsRow>
          <SettingsText>{getLocale('showTopSites')}</SettingsText>
          <Toggle
            onChange={toggleShowTopSites}
            checked={showTopSites}
            size='large'
          />
        </SettingsRow>
        <StyledTopSitesCustomizationSettings>
          <StyledTopSitesCustomizationSettingsOption>
            <StyledTopSitesCustomizationImageBorder
              selected={favoritesSelected}
            >
              <StyledTopSitesCustomizationImage
                imgSrcLight={favoritesSelected ? favoritesSelectedLight : favoritesUnselectedLight}
                imgSrcDark={favoritesSelected ? favoritesSelectedDark : favoritesUnselectedDark}
                selected={favoritesSelected}
                onClick={this.onClickFavorites}
              />
            </StyledTopSitesCustomizationImageBorder>
            <StyledTopSitesCustomizationOptionTitle>
              {getLocale('showFavoritesLabel')}
            </StyledTopSitesCustomizationOptionTitle>
            <StyledTopSitesCustomizationOptionDesc>
              {getLocale('showFavoritesDesc')}
            </StyledTopSitesCustomizationOptionDesc>
          </StyledTopSitesCustomizationSettingsOption>
          <StyledTopSitesCustomizationSettingsOption>
            <StyledTopSitesCustomizationImageBorder
              selected={frecencySelected}
            >
              <StyledTopSitesCustomizationImage
                imgSrcLight={frecencySelected ? frecencySelectedLight : frecencyUnselectedLight}
                imgSrcDark={frecencySelected ? frecencySelectedDark : frecencyUnselectedDark}
                selected={frecencySelected}
                onClick={this.onClickFrecency}
              />
            </StyledTopSitesCustomizationImageBorder>
            <StyledTopSitesCustomizationOptionTitle>
              {getLocale('showFrecencyLabel')}
            </StyledTopSitesCustomizationOptionTitle>
            <StyledTopSitesCustomizationOptionDesc>
              {getLocale('showFrecencyDesc')}
            </StyledTopSitesCustomizationOptionDesc>
          </StyledTopSitesCustomizationSettingsOption>
        </StyledTopSitesCustomizationSettings>
      </div>
    )
  }
}

export default TopSitesSettings
