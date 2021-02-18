// Copyright (c) 2019 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import styled, { css } from 'brave-ui/theme'

const isDarkTheme = (p: any) => {
  return p.theme.name === 'Brave Dark'
}

interface ListProps {
  blockNumber: number
}

export const List = styled<ListProps, 'div'>('div')`
  justify-self: start;
  align-items: normal;
  height: 100%;
  display: grid;
  grid-template-columns: repeat(${p => Math.min(p.blockNumber, 6).toString()}, 92px);
  justify-content: var(--ntp-item-justify, start);

  @media screen and (max-width: 700px) {
    grid-template-columns: repeat(${p => Math.min(p.blockNumber, 3).toString()}, 92px);
  }

  @media screen and (max-width: 390px) {
    grid-template-columns: repeat(${p => Math.min(p.blockNumber, 2).toString()}, 92px);
  }
`

export const TileActionsContainer = styled<{}, 'nav'>('nav')`
  box-sizing: border-box;
  opacity: 0;
  visibility: hidden;
  transition: 0.15s opacity linear;
  position: absolute;
  width: 32px;
  height: 32px;
  z-index: 1;
  top: -12px;
  right: -12px;
  display: flex;
`

export const TileMenu = styled<{}, 'div'>('div')`
  position: absolute;
  top: 15px;
  left: 80px;
  min-width: 185px;
  height: 72px;
  padding: 8px 0;
  background: ${p => isDarkTheme(p) ? '#3B3E4F' : '#FFF'};
  display: flex;
  flex-direction: column;
  border-radius: 4px;
  box-shadow: 0px 0px 16px 0px rgba(0, 0, 0, 0.3);
  z-index: 2;
`

export const TileMenuItem = styled<{}, 'div'>('div')`
  width: 100%;
  height: 30px;
  font-family: Poppins;
  font-size: 13px;
  font-style: normal;
  font-weight: 500;
  line-height: 20px;
  letter-spacing: 0.01em;
  text-align: left;
  padding: 4px 13px;
  background: inherit;
  display: flex;
  flex-direction: row;
  align-items: center;
  gap: 11px;
  color: ${p => p.theme.color.contextMenuHoverForeground};

  &:hover {
    background-color: ${p => p.theme.color.contextMenuHoverBackground};
    color: ${p => p.theme.color.contextMenuHoverForeground};
  }
`

export const TileAction = styled<{}, 'button'>('button')`
  -webkit-appearance: none;
  box-sizing: border-box;
  transition: color 0.1s linear;
  background: #DADCEB;
  width: 100%;
  border-radius: 50%;
  margin: 0;
  padding: 0;
  border: 0;
  cursor: pointer;
  display: flex;
  align-items: center;
  justify-content: center;
  outline: unset;

  &:focus {
    background: #AEB1C2;
  }
`

interface TileProps {
  isDragging: boolean
  isMenuShowing: boolean
}

export const Tile = styled<TileProps, 'div'>('div')`
  background-color: #ffffff;
  position: relative;
  user-select: none;
  margin: 6px;
  display: flex;
  justify-content: center;
  align-items: center;
  box-shadow: 1px 1px 6px 2px rgba(0,0,0,0.3);
  border-radius: 8px;
  width: 80px;
  height: 80px;
  font-size: 38px;
  cursor: grab;
  // Menu goes behind in other Tiles when tils has z-index.
  // Give z-index while dragging to make dragging tile moves over other tiles.
  z-index: ${p => p.isDragging ? 3 : 'unset' }

  ${p => !p.isDragging && !p.isMenuShowing && css`
    &:hover {
      ${TileActionsContainer} {
        opacity: 1;
        visibility: visible;
      }
    }
  `}

  ${p => p.isMenuShowing && css`
    ${TileActionsContainer} {
      opacity: 1;
      visibility: visible;
    }
  `}
`

export const TileFavicon = styled<{}, 'img'>('img')`
  display: block;
  height: 72px;
  padding: 16px;
  object-fit: contain;
`

export const ListWidget = List
